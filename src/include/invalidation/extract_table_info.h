#ifndef EXTRACT_TABLE_INFO_H
#define EXTRACT_TABLE_INFO_H

#include "postgres.h"
#include "parser/parsetree.h"

#define foreach(cell, l)        \
        for ((cell) = list_head(l); (cell) != NULL; (cell) = lnext(cell))

extern bool is_select_command(const char *query);          //returns true if it is a select command

extern char *skip_comment(const char *query);

extern void extract_table_oid_name(const char *, const char *, const List *, const char *);

#endif
