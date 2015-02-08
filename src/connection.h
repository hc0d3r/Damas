#ifndef _CON_H
#define _CON_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "damas.h"

void die(const char *err);
struct sockaddr_in make_addr(const char* name);
void start_listen(int *sock_fd, const char *ip_host, int port);


#endif
