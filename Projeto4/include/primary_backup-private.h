#ifndef _PRIMARY_BACKUP-PRIVATE_H
#define _PRIMARY_BACKUP-PRIVATE_H
struct server_t{
  int sockfd; //listening socket
	struct sockaddr_in dest;//
	int alive;
  char * addrs;
}
#endif
