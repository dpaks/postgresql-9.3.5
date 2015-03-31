/*
 * FILE: my_client.c
 * HEADER: invalidation/my_client.h
 *
 * Receives extracted query info and sends it to pqcd
 *
 * Written by Deepak S
 *
 * Copyright (c) 2015-Today	Deepak S (in.live.in@live.in)
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that copyright notice and this permission
 * notice appear in supporting documentation, and that the name of the
 * author not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission. The author makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 */

#include <string.h>
#include <stdlib.h>

#include "invalidation/my_client.h"
#include "invalidation/extract_table_info.h"

#define PORT "3490"         /* the port client will be connecting to */
#define MAXDATASIZE 250     /* max query size */
#define DBLENGTH 20
#define OIDLENGTH 6

/* Defined in extract_table_info.c too. Changes to be in sync */
#define TABLESNO 10
#define TOTOIDLENGTH (OIDLENGTH * TABLESNO)

/* get sockaddr, IPv4 or IPv6 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void client_main(const char *dbname, const char *query_old1,int *arr_oids,
                 const char *flag)
{
    int sockfd, numbytes;
    struct addrinfo hints, *servinfo, *p;
    int rv, i = 0;
    char s[INET6_ADDRSTRLEN];
    char relid[TOTOIDLENGTH] = "";
    char relid_temp[OIDLENGTH] = "";

    if (strncmp("/* cache:off */", query_old1, 15) == 0)
    {
        return;
    }

    query_old1 = skip_comment(query_old1);

    snprintf(relid_temp, OIDLENGTH, "%d", arr_oids[0]);
    relid_temp[OIDLENGTH-1] = '\0';
    strcpy(relid, relid_temp);
    for (i = 1; arr_oids[i] != 9999; i++)       /* oid array delimiter */
    {
        /*len of oid is 5. Hence, 5+1 (incl NULL) */
        snprintf(relid_temp, OIDLENGTH, "%d", arr_oids[i]);
        relid_temp[OIDLENGTH-1] = '\0';
        strcat(relid, relid_temp);
    }
    free(arr_oids);     /* no longer needed */

    int len = strlen((char *)query_old1) + strlen(relid) + strlen((char *)flag)
              + strlen((char *)dbname)+ 2;

    char *tableoid = malloc(len * sizeof(char));
    strcpy(tableoid, (char *)flag);
    strncat(tableoid, (char *)dbname, DBLENGTH);
    strcat(tableoid, ";");

    /*
     * Whatever it be, the query that is send will have a
     * max size of 250 chars inclusive of NULL
     */
    strncat(tableoid, (char *)query_old1, (MAXDATASIZE-1));
    if ( strlen(((char *)query_old1)) > (MAXDATASIZE-1))
    {
        strcat(tableoid, ";");
    }
    strcat(tableoid, relid);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    /* loop through all the results and connect to the first we can */
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);         /* all done with this structure */

    if (send(sockfd, tableoid, len, 0) == -1)
        perror("send");

    free(tableoid);
    close(sockfd);

    return;
}
