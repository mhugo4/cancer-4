#include "data.h"
#include "message-private.h"
#include "client_stub-private.h"
#include "inet.h"
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include "network_client.h"
#include "network_client-private.h"
#define MAX_KEYS 100 
/* Função para estabelecer uma associação entre o cliente e um conjunto de
 * tabelas remotas num servidor.
 * Os alunos deverão implementar uma forma de descobrir quantas tabelas
 * existem no servidor.
 * address_port é uma string no formato <hostname>:<port>.
 * retorna NULL em caso de erro .
 */
struct rtables_t *rtables_bind(const char *address_port){
	if(address_port == NULL){
		printf("port\n");
		return NULL;
	}
	
	struct rtables_t *rtable;
	if((rtable = (struct rtables_t*)malloc(sizeof(struct rtables_t)))==NULL){
		printf("rtable\n");
		return NULL;
	}
	
	struct server_t *server;
	if((server = (struct server_t*)malloc(sizeof(struct server_t)))==NULL){
		printf("server\n");
		return NULL;
	}
	rtable->server=server;
	/* Verificar parâmetro da função e alocação de memória */

	/* Estabelecer ligação ao servidor:

		Preencher estrutura struct sockaddr_in com dados do
		endereço do servidor.

		Criar a socket.

		Estabelecer ligação.
	*/
	
	// Cria socket TCP
	if ((rtable->server->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erro ao criar socket TCP");
		return NULL;
	}

	
	char *token[2];
	token[0] = strtok(address_port, ":");
	token[1] = strtok(NULL, "\0");
	/*Get host by name*/
	/*
	struct hostent *hostptr;
	if((hostptr= gethostbyname(token[0])) == NULL) {
		printf("get host by name error for host: %s\n", token[0]);
		return NULL;
	}
	/* Get service by port 
	struct servent *hostptrs;
	printf("%d\n", atoi(token[1]));
	if((hostptrs= getservbyport(atoi(token[1]),"TCP")) == NULL) {
		printf("get service by port error for host: %s\n", token[1]);
		return NULL;
	}
	// Preenche estrutura server para estabelecer conexão
	struct sockaddr_in servers;
	servers.sin_family = hostptr->h_addrtype;
	servers.sin_port = hostptrs->s_port;
	bzero(servers.sin_zero,8);
	*/
	struct sockaddr_in servers;
	servers.sin_family = AF_INET;
	servers.sin_port = htons(atoi(token[1]));
	if (inet_pton(AF_INET, token[0], &servers.sin_addr) < 1) {
		printf("Erro ao converter IP\n");
		close(server->sockfd);
		return NULL;
	}
	rtable->server->dest=servers;


	/* Se a ligação não foi estabelecida, retornar NULL */
	// Estabelece conexão com o servidor definido em server
	if (connect(rtable->server->sockfd,(struct sockaddr *)&(servers), sizeof(servers)) < 0) {
		perror("Erro ao conectar-se ao servidor");
		close(rtable->server->sockfd);
		return NULL;
	}
	return rtable;

}

/* Termina a associação entre o cliente e um conjunto de tabelas remotas, e
 * liberta toda a memória local.
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtables_unbind(struct rtables_t *rtables){
	if(rtables==NULL){
		return -1;
	}
	network_close(rtables->server);
	free(rtables);
	return 0;
}

/* Função para adicionar um par chave valor numa tabela remota.
 * Devolve 0 (ok) ou -1 (problemas).
 */
int rtables_put(struct rtables_t *rtables, char *key, struct data_t *value){

	struct message_t *msg_out, *msg_resposta;
	if((msg_out = (struct message_t*)malloc(sizeof(struct message_t)))==NULL){
		return -1;
	}
	msg_out->opcode = OC_PUT;
	msg_out->c_type = CT_ENTRY;
	msg_out->table_num = rtables->actTable;


	if((msg_out->content.entry = (struct entry_t* )malloc(sizeof(struct entry_t)))==NULL){
		free_message(msg_out);
		return -1;
	}
	entry_initialize(msg_out->content.entry);
	msg_out->content.entry->key=key;
	msg_out->content.entry->value=value;
	msg_resposta = network_send_receive(rtables->server,msg_out);
	free_message(msg_out);
	if(msg_resposta==NULL){
		return -1;
	}	
	print_message(msg_resposta);
	int x =msg_resposta->content.result;
	free_message(msg_resposta);
	return x;  

}

/* Função para substituir na tabela remota, o valor associado à chave key.
 * Devolve 0 (OK) ou -1 em caso de erros.
 */
int rtables_update(struct rtables_t *rtables, char *key, struct data_t *value){

	struct message_t *msg_out, *msg_resposta;
	if((msg_out = (struct message_t*)malloc(sizeof(struct message_t)))==NULL){
		return -1;
	}
	msg_out->opcode = OC_UPDATE;
	msg_out->c_type = CT_ENTRY;
	msg_out->table_num = rtables->actTable;

	if((msg_out->content.entry = (struct entry_t* )malloc(sizeof(struct entry_t)))==NULL){
		free_message(msg_out);
		return -1;
	}

	entry_initialize(msg_out->content.entry);
	msg_out->content.entry->key=key;
	msg_out->content.entry->value=value;

	msg_resposta = network_send_receive(rtables->server,msg_out);
	free_message(msg_out);
	if(msg_resposta==NULL){
		return -1;
	}	
	print_message(msg_resposta);
	int x =msg_resposta->content.result;
	free_message(msg_resposta);
	return x;  
}

/* Função para obter da tabela remota o valor associado à chave key.
 * Devolve NULL em caso de erro.
 */
struct data_t *rtables_get(struct rtables_t *rtables, char *key){

	struct message_t *msg_out, *msg_resposta;
	if((msg_out = (struct message_t*)malloc(sizeof(struct message_t)))==NULL){
		return NULL;
	}
	msg_out->opcode = OC_GET;
	msg_out->c_type = CT_KEY;
	msg_out->table_num = rtables->actTable;
	msg_out->content.key=key;
	msg_resposta = network_send_receive(rtables->server,msg_out);
	free_message(msg_out);
	if(msg_resposta==NULL){
		return NULL;
	}
	print_message(msg_resposta);
	
	
	struct data_t *data = data_dup(msg_resposta->content.data);
	if (data==NULL){
		printf(" data null\n");
		return NULL;
	}
	free_message(msg_resposta);
	return data; 
}

/* Devolve número de pares chave/valor na tabela remota.
 */
int rtables_size(struct rtables_t *rtables){

	struct message_t *msg_out, *msg_resposta;
	if((msg_out = (struct message_t*)malloc(sizeof(struct message_t)))==NULL){
		return -1;
	}
 	msg_out->opcode = OC_SIZE;
	msg_out->c_type = CT_RESULT;
	msg_out->table_num = rtables->actTable;
	msg_out->content.result = rtables->actTable;
	msg_resposta = network_send_receive(rtables->server,msg_out);
	free_message(msg_out);
	if(msg_resposta==NULL){
		return -1;
	}
	print_message(msg_resposta);
	int x =msg_resposta->content.result;
	free_message(msg_resposta);
	return x; 
}
/* Devolve o número de colisões existentes na tabela remota.
 */
int rtables_collisions(struct rtables_t *rtables){

	struct message_t *msg_out, *msg_resposta;

	if((msg_out = (struct message_t*)malloc(sizeof(struct message_t)))==NULL){
		return-1;
	}
	msg_out->opcode = OC_COLLS;
	msg_out->c_type = CT_RESULT;
	msg_out->table_num = rtables->actTable;
	msg_out->content.result = msg_out->table_num;
	msg_resposta = network_send_receive(rtables->server,msg_out);
	free_message(msg_out);
	if(msg_resposta==NULL){
		return -1;
	}	
	print_message(msg_resposta);
	int x =msg_resposta->content.result;
	free_message(msg_resposta);
	return x;  
}


/* Devolve um array de char * com a cópia de todas as keys da
 * tabela remota, e um último elemento a NULL.
 */
char **rtables_get_keys(struct rtables_t *rtables){

	struct message_t *msg_out, *msg_resposta;

	if((msg_out = (struct message_t*)malloc(sizeof(struct message_t)))==NULL){
		return NULL;
	}
 	msg_out->opcode = OC_GET;
	msg_out->c_type = CT_KEY;
	msg_out->table_num = rtables->actTable;
	msg_out->content.key ="*" ;
	msg_resposta = network_send_receive(rtables->server,msg_out);
	//free_message(msg_out);
	if(msg_resposta==NULL){
		return NULL;
	}
	print_message(msg_resposta);
	char **ret;
    if((ret=malloc(sizeof(char)*MAX_KEYS))==NULL){
        return NULL;
    }
    int i;
    for (i = 0; msg_resposta->content.keys[i] != NULL; i++)
    {
    	if(msg_resposta->content.keys[i]!=NULL)
        ret[i]=msg_resposta->content.keys[i];

    }
    //free_message(msg_out);
    free_message(msg_resposta);
    return ret; 
}

/* Liberta a memória alocada por rtables_get_keys().
 */
void rtables_free_keys(char **keys){
	free(keys);
}
