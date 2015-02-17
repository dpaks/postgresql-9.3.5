/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright (c) 2000-2013, PostgreSQL Global Development Group
 *
 * src/bin/psql/mainloop.h
 */
#ifndef MAINLOOP_H
#define MAINLOOP_H

//#include "invalidation/my_client.h"
#include "postgres_fe.h"

int			MainLoop(FILE *source);

//int is_meta();

#endif   /* MAINLOOP_H */
