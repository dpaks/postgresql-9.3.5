/*
 * FILE: extract_table_info.c
 * HEADER: invalidation/extract_table_info.h
 *
 * Retrieves query info and rtable from Postgres.c and sends it to a client
 * file which sends it to pqcd. Table OID(s) of a query are extracted from
 * rtable here.
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

#include <ctype.h>
#include <limits.h>

#include "invalidation/extract_table_info.h"
#include "invalidation/my_client.h"

#define TABLESNO 10     /* Assuming max 10 tables are referenced by a query */

/*
 * Checking if it is a select query
 */
bool is_select_command(const char *query)
{

    /* skip comment */
    query = skip_comment(query);

    if (*query == '\0')
        return false;

    /* skip spaces */
    while (*query && isspace(*query))
        query++;

    /* SELECT? */
    if (strncasecmp("SELECT", query, 6))	/* returns false if a match */
        return false;

    return true;				/* returns true if it is a select command */
}

/*
 * Skipping comments of a query
 */
char *skip_comment(const char *query)
{
    if (strncmp(query, "/*", 2) == 0)
    {
        query += 2;
        while (query)
        {
            if (strncmp(query, "*/", 2) == 0)
            {
                query += 2;
                break;
            }
            query++;
        }
    }

    /* skip spaces */
    while (isspace(*query))
        query++;
    return (char *)query;
}

/*
 * Sending table name, db name, oid(s) and flag to client
 */
void extract_table_oid_name(const char *dbname, const char *query,
                            const List *rtable, char *flag)
{
    const ListCell *l;
    int *arr_oids = malloc(sizeof(int) * TABLESNO);
    int i = 0;

    foreach(l, rtable)
    {
        RangeTblEntry *rte = lfirst(l);

        /*
         * User generated tables have an oid > 9999
         */
        if ((int)rte->relid > 9999)
        {
            arr_oids[i++] = (int)rte->relid;
        }
    }
    arr_oids[i] = 55555;       /* Array terminator is 55555 */

    client_main(dbname, query, arr_oids, flag);
}
