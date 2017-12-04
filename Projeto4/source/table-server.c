//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34
/*
   Programa que implementa um servidor de uma tabela hash com chainning.
   Uso: table-server <port> <table1_size> [<table2_size> ...]
   Exemplo de uso: ./table_server 54321 10 15 20 25
*/
#include <error.h>
#include "inet.h"
#include "table-private.h"
#include "table.h"
#include "message.h"
#include "message-private.h"
#include "network_client-private.h"
#include <signal.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#define NFDESC 4 // Numero de sockets (uma para listening)
#define TIMEOUT 50 // em milisegundos

/* Função para preparar uma socket de receção de pedidos de ligação.
*/


int make_server_socket(short port){
  int socket_fd;
  struct sockaddr_in server;

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    perror("Erro ao criar socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_fd, (struct sockaddr *) &server, sizeof(server)) < 0){
      perror("Erro ao fazer bind");
      close(socket_fd);
      return -1;
  }

  if (listen(socket_fd, 0) < 0){
      perror("Erro ao executar listen");
      close(socket_fd);//ESTAVA (sfd)
      return -1;
  }

  return socket_fd;
}


/* Função que recebe uma tabela e uma mensagem de pedido e:
	- aplica a operação na mensagem de pedido na tabela;
	- devolve uma mensagem de resposta com oresultado.
*/
struct message_t *process_message(struct message_t *msg_pedido, struct table_t *tabela){
	///struct message_t *msg_resposta;

	/* Verificar parâmetros de entrada */
	if(msg_pedido == NULL || tabela == NULL){
		return NULL;
	}

	struct message_t *msg_resposta =(struct message_t*) malloc(sizeof(struct message_t));
	/* Verificar opcode e c_type na mensagem de pedido */
	short op = msg_pedido->opcode;
	/* Aplicar operação na tabela */
	switch(op){

		case OC_SIZE:
		msg_resposta->content.result=tabela->size;
		msg_resposta->c_type=CT_RESULT;
		break;

		case OC_PUT:

		msg_resposta->content.result=table_put(tabela,msg_pedido->content.entry->key,msg_pedido->content.entry->value);
		msg_resposta->c_type=CT_RESULT;
		break;

		case OC_GET:

		if(strcmp(msg_pedido->content.key,"*")==0){

			msg_resposta->content.keys=table_get_keys(tabela);
			msg_resposta->c_type=CT_KEYS;
			break;
		}

		struct data_t *ret=table_get(tabela,msg_pedido->content.key);
		if(ret==NULL){
			msg_resposta->content.result=-1;
			msg_resposta->c_type=CT_RESULT;
			data_destroy(ret);
			break;
		}
		msg_resposta->content.data=data_create2(ret->datasize,ret->data);
		msg_resposta->c_type=CT_VALUE;
		data_destroy(ret);
		break;

		case OC_UPDATE:

		msg_resposta->content.result=table_update(tabela,msg_pedido->content.entry->key,msg_pedido->content.entry->value);
		msg_resposta->c_type=CT_RESULT;
		break;

		case OC_COLLS:

		msg_resposta->content.result=tabela->numCols;
		msg_resposta->c_type=CT_RESULT;
		break;


	}
	/* Preparar mensagem de resposta */
	msg_resposta->opcode = op+1;
	msg_resposta->table_num=msg_pedido->table_num;
	return msg_resposta;
}


/* Função "inversa" da função network_send_receive usada no table-client.
   Neste caso a função implementa um ciclo receive/send:

	Recebe um pedido;
	Aplica o pedido na tabela;
	Envia a resposta.
*/
int network_receive_send(int sockfd, struct table_t *table){
  char *message_resposta, *message_pedido;
  int msg_length;
  int message_size, msg_size, result;
  struct message_t *msg_pedido, *msg_resposta;

	/* Verificar parâmetros de entrada */
  	if(sockfd < 0 || table == NULL){
  		return -1;
  	}

	/* Com a função read_all, receber num inteiro o tamanho da
	   mensagem de pedido que será recebida de seguida.*/
	result = read_all(sockfd, (char *) &msg_size, _INT);

	/* Verificar se a receção teve sucesso */
	if (result != _INT){
		return -1;
	}

	msg_size = ntohl(msg_size);
	/* Alocar memória para receber o número de bytes da
	   mensagem de pedido. */
	message_pedido = (char*)malloc(msg_size);

	/* Com a função read_all, receber a mensagem de resposta. */
	result = read_all(sockfd, message_pedido, msg_size);

	/* Verificar se a receção teve sucesso */
	if (result != msg_size){
		return -1;
	}

	/* Desserializar a mensagem do pedido */
	msg_pedido = buffer_to_message(message_pedido, msg_size);

	/* Verificar se a desserialização teve sucesso */
	if(msg_pedido == NULL){
		return -1;
	}
	/* Processar a mensagem */

    msg_resposta = invoke(msg_pedido);
	//msg_resposta = process_message(msg_pedido, tables);

	/* Serializar a mensagem recebida */
	message_size = message_to_buffer(msg_resposta, &message_resposta);

	/* Verificar se a serialização teve sucesso */
	if(message_size == -1){
		return -1;
	}
	/* Enviar ao cliente o tamanho da mensagem que será enviada
	   logo de seguida
	*/

	msg_size = htonl(message_size);
 	//result = write_all(server->/*atributo*/, (char *) &msg_size, _INT));// como o prof tinha
	  result = write_all(sockfd, (char *) &msg_size, _INT);
	/* Verificar se o envio teve sucesso */
	  if(result != _INT){
	  	return -1;
	  }
	/* Enviar a mensagem que foi previamente serializada */

	//result = write_all(server->/*atributo*/, message_resposta, message_size);//COMO O PROF TINHA
	result = write_all(sockfd, message_resposta, message_size);


	/* Verificar se o envio teve sucesso */
	if (result != message_size)
	{
		return -1;
	}
	/* Libertar memória */

	print_message(msg_resposta);
	free(message_resposta);
	free(message_pedido);
	free_message(msg_resposta);
	free_message(msg_pedido);
	return 0;
}


int main(int argc, char **argv){
  int listening_socket, connsock, nfds, kfds, res, fatal_error, nbytes, result;
  struct sockaddr_in client;
  socklen_t size_client;
  struct pollfd connections[NFDESC+2];
  char *buffer;
  struct message_t *message, *msg_out;
  char *buf;
  char input[81];
  char **tables;
  if((tables=malloc((argc-2)*sizeof(int)))==NULL){
  	printf("Falhou o Malloc de tables\n");
  	return -1;
  }

  if (argc < 4){
	printf("Uso: ./server <porta TCP> <port> <table1_size> [<table2_size> ...]\n");
	printf("Exemplo de uso: ./table-server 54321 127.0.0.1:54322 10 15 20 25\n");
	return -1;
  }
  signal(SIGPIPE, SIG_IGN);
  size_client = sizeof((struct sockaddr *) &client);
  if ((listening_socket = make_server_socket(atoi(argv[1]))) < 0){
	return -1;
  }
  /*********************************************************/
  /* Criar as tabelas de acordo com linha de comandos dada */
  /*********************************************************/
  int i;
  int j=1;
  sprintf(&tables[0],"%d",argc-3);
  for(i=3;i<argc;i++,j++){
	   tables[j]=argv[i];
  }
  table_skel_init(tables);



  for (i = 0; i < NFDESC; i++){
    connections[i].fd = -1; // poll ignora estruturas com fd < 0
  }
  connections[0].fd = listening_socket;  // Vamos detetar eventos na welcoming socket
  connections[0].events = POLLIN;  		 // Vamos esperar ligacoes nesta socket
  connections[1].fd = fileno(stdin);
  connections[1].events = POLLIN;


  char *aux;
  kfds=0;
  nfds = 2; // numero de file descriptors
  printf("Waiting for Client\n");
  while((kfds = poll(connections, nfds, -1)) >= 0){ // kfds == 0 significa timeout sem eventos

		if (kfds > 0){ // kfds é o número de descritores com evento ou erro
				/* espera por dados nos sockets abertos */
			res = poll(connections, nfds, 10);
			if (res<0){
				printf("res < 0 \n");
			  if (errno != EINTR){
				fatal_error = 1;
				continue;
			  }
			}



			if((connections[0].revents & POLLIN) && (nfds < NFDESC)){
				/* novo pedido de conexão */

				if((connsock= accept(listening_socket, (struct sockaddr *) &client, &size_client))==-1){
					printf("Sem accept\n");
					return -1 ;

				}

				if(connections[nfds].fd==-1){
				  connections[nfds].fd=connsock;
				  connections[nfds].events=POLLIN;
				  connections[nfds].revents=POLLIN;
				  nfds++;
				}

			}
			if((connections[1].revents & POLLIN) && (nfds < NFDESC)){

				aux = fgets(input,80,stdin);
				input[strcspn(input,"\n")]='\0';

				if(strncmp(input,"print",strlen("print"))==0){
			  	/
			  		strtok(aux," ");
			  		int a = atoi(strtok(NULL,"\n"));

			  		table_skel_print(a);
			  	}if(strncmp(input,"quit",strlen("quit"))==0){
					table_skel_destroy();
					for (i = 1; i < nfds; i++)	/*for each socket s em conjunto_de_descritores{*/
						close(connections[i].fd);
					free_message(msg_out);
					free_message(message);
					free(buffer);
					free(tables);
					free(buf);
					return 0;
				}
			}

			/* um dos sockets de ligação tem dados para ler */
			for (i = 2; i < nfds; i++){ // Todas as ligações /*for each socket_de_cliente, s,em conjunto_de_descritores*/
			    if (connections[i].revents & POLLIN) {	/*s tem dado(s para ler codigo exemplo stor*/ //revents

			  		nbytes = read_all(connections[i].fd,(char *) &result, _INT);	/*nbytes= read_all(s, buffer, ...);*/

					if(nbytes==0) {
					  /* sinal de que a conexão foi fechada pelo cliente */
						close(connections[i].fd);	/*close(s)*/
						for(i=1;i<NFDESC;i++){
							if(connections[i].fd==connsock){
								connections[i].fd=-1;
							  	nfds--;
							  	continue;
							}
					  	}
					}else{
						result = ntohl(result);
					/* Alocar memória para receber o número de bytes da mensagem de pedido. */
		   				buffer = (char*)malloc(result);

					/* Com a função read_all, receber a mensagem de resposta. */
						nbytes = read_all(connections[i].fd, buffer, result);

					 	if(nbytes==0) {
					 		/* sinal de que a conexão foi fechada pelo cliente */
						  	close(connections[i].fd);	/*close(s)*/
						 	for(i=1;i<NFDESC;i++){
								if(connections[i].fd==connsock){
									connections[i].fd=-1;
									nfds--;
									continue;
								}
						  	}
					  	}
						  /* processamento da requisição e da resposta */
						message = buffer_to_message(buffer,result);	/*message = buffer_to_message(buffer);*/
						msg_out = invoke(message);
						res = message_to_buffer(msg_out,&buf);	/*buffer = message_to_buffer(msg_out);*/
						result = htonl(res);

						nbytes=write_all(connections[i].fd,(char*)&result, _INT);	/*write_all(s,buffer, ...)*/

						if(nbytes==0) {
							/* sinal de que a conexão foi fechada pelo cliente */
							close(connections[i].fd);	/*close(s)*/
							for(i=1;i<NFDESC;i++){
								if(connections[i].fd==connsock){
									connections[i].fd=-1;
									nfds--;
									continue;
								}
							}
						}

					    nbytes=write_all(connections[i].fd,buf,res);

					  	if(nbytes==0) {
						    /* sinal de que a conexão foi fechada pelo cliente */
						  	close(connections[i].fd);	/*close(s)*/
						  	for(i=1;i<NFDESC;i++){
								if(connections[i].fd==connsock){
							  		connections[i].fd=-1;
							  		nfds--;
							  		continue;
								}
						  	}
						}
					    if (connections[i].revents & POLLHUP) {	/*s com erro ou POLLHUP*/

							close(connections[i].fd);	/*close(s);*/
							connections[i].fd=-1;	/*remove s de conjunto_de_descritores*/
							nfds--;
					  	}

				}
			}

		}
	}

  }
 	return 0;
}
