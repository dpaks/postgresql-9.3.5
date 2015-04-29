#ifndef MY_CLIENT_H
#define MY_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

extern void *get_in_addr(struct sockaddr *sa);

extern void client_main(const char *, const char *,int *, char *);

#endif
