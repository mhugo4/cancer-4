
//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34
#include "network_client-private.h"
#include <stdlib.h>
#include <errno.h>
#include "message.h"
#include "inet.h"
#include <signal.h>


void print_message(struct message_t *msg) {
    int i;

    printf("\n----- MESSAGE -----\n");
    printf("Tabela número: %d\n", msg->table_num);
    printf("opcode: %d, c_type: %d\n", msg->opcode, msg->c_type);
    switch(msg->c_type) {
        case CT_ENTRY:{
            printf("key: %s\n", msg->content.entry->key);
            printf("datasize: %d\n", msg->content.entry->value->datasize);
        }break;
        case CT_KEY:{
            printf("key: %s\n", msg->content.key);
        }break;
        case CT_KEYS:{
            for(i = 0; msg->content.keys[i] != NULL; i++) {
                printf("key[%d]: %s\n", i, msg->content.keys[i]);
            }
        }break;
        case CT_VALUE:{
            printf("datasize: %d\n", msg->content.data->datasize);
        }break;
        case CT_RESULT:{
            printf("result: %d\n", msg->content.result);
        }break;
        case OC_RT_ERROR:{
            printf("result: %d\n", msg->content.result);
        };
    }
    printf("-------------------\n");
}

int write_all(int sock, char *buf, int len){
	int bufsize = len;
	while(len>0) {
		int res = write(sock, buf, len);
		if(res<0) {
			if(errno==EINTR) continue;
			perror("write failed:");
			return res;
			}
		buf += res;
		len -= res;
	}
	return bufsize;
}

int read_all(int sock, char *buf, int len){
	int bufsize = len;
	while(len>0) {
		int res = read(sock, buf, bufsize);
		if(res<0) {
			if(errno==EINTR) continue;
			perror("read failed:");
			return res;
			}
		buf += res;
		len -= res;
	}
	return bufsize;
}

/* Esta função deve:
 *    - estabelecer a ligação com o servidor;
 *    - address_port é uma string no formato <hostname>:<port>
 *      (exemplo: 10.10.10.10:10000)
 *    - retornar toda a informacão necessária (e.g., descritor da
 *      socket) na estrutura server_t
 */
struct server_t *network_connect(const char *address_port){


	if(address_port == NULL){
		printf("port\n");
		return NULL;
	}
	struct server_t *server;
	if((server = (struct server_t*)malloc(sizeof(struct server_t)))==NULL){
		printf("server\n");
		return NULL;
	}
	/* Verificar parâmetro da função e alocação de memória */



	/* Estabelecer ligação ao servidor:

		Preencher estrutura struct sockaddr_in com dados do
		endereço do servidor.

		Criar a socket.

		Estabelecer ligação.
	*/
	// Cria socket TCP
	if ((server->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erro ao criar socket TCP");
		return NULL;
	}


	char *token[2];
	token[0] = strtok(address_port, ":");
	token[1] = strtok(NULL, "\0");

	// Preenche estrutura server para estabelecer conexão
	struct sockaddr_in servers;
	servers.sin_family = AF_INET;
	servers.sin_port = htons(atoi(token[1]));
	if (inet_pton(AF_INET, token[0], &servers.sin_addr) < 1) {
		printf("Erro ao converter IP\n");
		close(server->sockfd);
		return NULL;
	}



	/* Se a ligação não foi estabelecida, retornar NULL */
	// Estabelece conexão com o servidor definido em server
	if (connect(server->sockfd,(struct sockaddr *)&(servers), sizeof(servers)) < 0) {
		perror("Erro ao conectar-se ao servidor");
		close(server->sockfd);
		return NULL;
	}


	return server;
}


/* Esta função deve
 *   - Obter o descritor da ligação (socket) da estrutura server_t;
 *   - enviar a mensagem msg ao servidor;
 *   - receber uma resposta do servidor;
 *   - retornar a mensagem obtida como resposta ou NULL em caso
 *     de erro.
 */
struct message_t *network_send_receive(struct server_t *server, struct message_t *msg){
	char *message_out;
	int message_size, msg_size, result;
	struct message_t *msg_resposta;

	/* Verificar parâmetros de entrada */
	if(server == NULL ){
		printf("SERVER  NULL\n");
		return NULL;

	}
	if( msg == NULL){
		printf("MESSAGE NULL\n");
		return NULL;
	}

	/* Serializar a mensagem recebida */
	message_size = message_to_buffer(msg, &message_out);

	/* Verificar se a serialização teve sucesso */
	if(message_size == -1)
		return NULL;

	/* Enviar ao servidor o tamanho da mensagem que será enviada
	   logo de seguida
	*/
	msg_size = htonl(message_size);

 	result = write_all(server->sockfd,(char*)&msg_size, _INT);

	/* Verificar se o envio teve sucesso */
 	if(result !=  _INT)
 		return NULL;
	/* Enviar a mensagem que foi previamente serializada */



	result = write_all(server->sockfd, message_out, message_size);


	/* Verificar se o envio teve sucesso */
	if(result !=  message_size){
		sleep(RETRY_TIME);
		// FAZER
		char *ip = inet_ntoa(server->dest.sin_addr);
		char *port = htons(server->dest.sin_port);
		char *addr = strcat(ip,":");
		addr = strcat(addr,port);
		server = network_connect(addr);
		result = write_all(server->sockfd, message_out, message_size);
		if(result == -1)
		{
			return NULL;
		}
	}


	/* De seguida vamos receber a resposta do servidor:

		Com a função read_all, receber num inteiro o tamanho da
		mensagem de resposta.

		Alocar memória para receber o número de bytes da
		mensagem de resposta.

		Com a função read_all, receber a mensagem de resposta.

	*/
 	free(message_out);
 	printf("Waiting for the Server\n");
	result = read_all(server->sockfd,(char*)&msg_size , _INT);
	msg_size = ntohl(msg_size);
	if(result !=  _INT){
		sleep(RETRY_TIME);
		// FAZER
		char *ip = inet_ntoa(server->dest.sin_addr);
		char *port = htons(server->dest.sin_port);
		char *addr = strcat(ip,":");
		addr = strcat(addr,port);
		server = network_connect(addr);
		result = write_all(server->sockfd, message_out, message_size);
		if(result == -1)
		{
			return NULL;
		}
	}
	message_out = (char*)malloc(msg_size);

	if(message_out == NULL){
		printf("message_out NULL \n");
		return NULL;
	}
	result =  read_all(server->sockfd, message_out, msg_size);

	/* Desserializar a mensagem de resposta */
	msg_resposta = buffer_to_message( message_out, msg_size );

	/* Verificar se a desserialização teve sucesso */
	if(msg_resposta == NULL){
		printf("msg_resposta NULL \n");

		return NULL;
	}
	/* Libertar memória */
	free(message_out);
	return msg_resposta;
}

/* A funcao network_close() deve fechar a ligação estabelecida por
 * network_connect(). Se network_connect() alocou memoria, a função
 * deve libertar essa memoria.
 */
int network_close(struct server_t *server){
	/* Verificar parâmetros de entrada */
	if(server == NULL)
		return -1;

	/* Terminar ligação ao servidor */
	close(server->sockfd);
	/* Libertar memória */


	free(server);
	return 0;
}
