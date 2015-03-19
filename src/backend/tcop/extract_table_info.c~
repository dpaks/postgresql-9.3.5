
#include "invalidation/extract_table_info.h"
#include "invalidation/my_client.h"
//#include "utils/elog.h"             //ereport defined here
/*#include <stdio.h>

#include "executor/tuptable.h"

#include "postgres.h"

#include "access/printtup.h"
#include "lib/stringinfo.h"
#include "nodes/print.h"
#include "optimizer/clauses.h"
#include "parser/parsetree.h"
#include "utils/lsyscache.h"
#include <stdbool.h>*/

/***********************Checking if it is a select query************************/

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
        if (strncasecmp("SELECT", query, 6))	//returns false if a match
                return false;

        return true;				//returns true if it is a select command
}

/**************************Skipping comments of a query**********************************/

/* skip SQL comments */
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
	return query;
}

/*******************Extracting table name and oid and writing to a file**********************/

void extract_table_oid_name(const char *query, const List *rtable, const char *flag)
{
    const ListCell *l;


            foreach(l, rtable)
                {
                    RangeTblEntry *rte = lfirst(l);

                    if ((int)rte->relid > 10000)
                        client_main(query, (int)rte->relid, flag);        //flag = f; (invalidateable)
                                                                      //flag = t; (cacheable)

                }

}

/*************************************************************************************************/

