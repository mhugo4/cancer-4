//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34
#ifndef _NETWORK_CLIENT_PRIVATE_H
#define _NETWORK_CLIENT_PRIVATE_H
#define RETRY_TIME 2

#include "inet.h"
#include "network_client.h"

struct server_t{
	int sockfd; //listening socket
	struct sockaddr_in dest;
	int alive;
  char * addrs;

};
/* Função que garante o envio de len bytes armazenados em buf,
   através da socket sock.
*/
int write_all(int sock, char *buf, int len);

/* Função que garante a receção de len bytes através da socket sock,
   armazenando-os em buf.
*/
int read_all(int sock, char *buf, int len);

void print_message(struct message_t *msg);


#endif
