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

#define PORT "3490" // the port client will be connecting to

void *get_in_addr(struct sockaddr *sa);

void client_main(const char *query_old1,int relid, const char *flag);

//void psql_client_main(char *query_old1);

#endif
