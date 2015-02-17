#ifndef EXTRACT_TABLE_INFO_H
#define EXTRACT_TABLE_INFO_H

#include "postgres.h"
#include "parser/parsetree.h"

#define foreach(cell, l)        \
        for ((cell) = list_head(l); (cell) != NULL; (cell) = lnext(cell))

       // #define ereport(elevel, rest)   ereport_domain(elevel, TEXTDOMAIN, rest)


bool is_select_command(const char *query);          //returns true if it is a select command

char *skip_comment(const char *query);

void extract_table_oid_name(const char *, const List *, const char *);     //flag = f; (invalidateable)
                                                                      //flag = t; (cacheable)

#endif
