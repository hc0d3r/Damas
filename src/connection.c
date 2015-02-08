#include "connection.h"

void die(const char *err){
	endwin();
        if(errno)
                perror(err);
        else
            	fprintf(stderr, "%s\n",err);
        exit(1);
}

struct sockaddr_in make_addr(const char* name){
	struct hostent *host = gethostbyname(name);
	struct sockaddr_in addr;

	if(host == NULL)
		die("gethostbyname() error");

	addr.sin_family = host->h_addrtype;
	addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(addr.sin_zero), 8);

	return addr;
}


void start_listen(int *sock_fd, const char *ip_host, int port){
	struct sockaddr_in server_addr;
	int optval = 1;

	if( (*sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		die("socket() error");

	server_addr = make_addr(ip_host);
	server_addr.sin_port = htons(port);

	if(setsockopt(*sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) == -1)
		die("setsockopt() error");

	if(bind(*sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
		die("bind() error");

	if(listen(*sock_fd,1) == -1)
		die("listen() error");

}

