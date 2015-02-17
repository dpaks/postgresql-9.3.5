/*
** client.c -- a stream socket client demo
*/

#include "invalidation/my_client.h"

#define len_relid 6 // length of relid



// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void client_main(const char *query_old1,int relid_old, const char *flag)
{
    int sockfd, numbytes;
    //char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv, i;
    char s[INET6_ADDRSTRLEN];

    char *query = (char *)query_old1;
    for(i = 0; query[i] != ';'; i++);
    int len_query = i+1;
    char relid[len_relid];
    snprintf(relid,len_relid, "%d", relid_old);

    int len = len_query + len_relid + 2 + 1;        //length of flag value is set as 2
    char *tableoid = malloc(len);
    strcpy(tableoid, (char *)flag);
    strcat(tableoid, query_old1);
    strcat(tableoid, relid);

    /*int len = len_relid + 1;        //length of flag value is set as 2
    char *tableoid = malloc(len);
    strcpy(tableoid, relid);*/

    /*if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }*/

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    if (send(sockfd, tableoid, len, 0) == -1)
                perror("send");

    close(sockfd);

    return 0;
}
