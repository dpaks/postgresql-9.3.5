#include <ctype.h>

#include "invalidation/extract_table_info.h"
#include "invalidation/my_client.h"

#define TABLESNO 10     /* Assuming max 10 tables referenced by a query */

/*
 *Checking if it is a select query
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
 *Skipping comments of a query
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
 *Sending table name, db name, oid(s) and flag to client
 */
void extract_table_oid_name(const char *dbname, const char *query, const List *rtable, const char *flag)
{
    const ListCell *l;
    int *arr_oids = malloc(sizeof(int) * TABLESNO);
    int i = 0;

    foreach(l, rtable)
    {
        RangeTblEntry *rte = lfirst(l);

        /*
         * User generated tables ahave an oid > 9999
         */
      //  if ((int)rte->relid > 9999)
        {
            arr_oids[i++] = (int)rte->relid;
        }
    }
    arr_oids[i] = 9999;       /* Array terminator is 9999 */

    if (i >= 1)
    {
        client_main(dbname, query, arr_oids, flag);
    }
}
