
//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34
/*
	Programa cliente para manipular tabela de hash remota.
	Os comandos introduzido no programa não deverão exceder
	80 carateres.

	Uso: table-client <ip servidor>:<porta servidor>
	Exemplo de uso: ./table_client 10.101.148.144:54321
*/
// nao teremos de fazer um table create?

#include "network_client-private.h"
#include "client_stub-private.h"
#include "message.h"
#include "entry.h"
#include <errno.h>
#include <inet.h>
#include <signal.h>

int main(int argc, char **argv){
	int connect =1;
	int exec = 1;
	int temp = 0;
	char input[81];
	char *aux;
	struct rtables_t *rtable;
	struct rtables_t *rtablesec;
	struct message_t *msg_out, *msg_resposta;
	/* Testar os argumentos de entrada */
	if (argc != 2){
		printf("Uso: ./client <ip_servidor> <porto_servidor>\n");
		printf("Exemplo de uso: ./client 127.0.0.1:12345 127.0.0.1:12345\n");
		return -1;
	}
	signal(SIGPIPE, SIG_IGN);
	/* Usar network_connect para estabelcer ligação ao servidor */
	if((rtable = rtables_bind(argv[1]))==NULL){
		return -1;
	}
	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
 	while (exec){
		printf(">>> "); // Mostrar a prompt para inserção de comando
		/* Receber o comando introduzido pelo utilizador
		   Sugestão: usar fgets de stdio.h
		   Quando pressionamos enter para finalizar a entrada no
		   comando fgets, o carater \n é incluido antes do \0.
		   Convém retirar o \n substituindo-o por \0.
		*/
		aux = fgets(&input,80,stdin);
		strtok(aux," ");
		/* Verificar se o comando foi "quit". Em caso afirmativo
		   não há mais nada a fazer a não ser terminar decentemente.
		*/
		if(strcmp(aux,"quit\n")==0){
			exec = 0;
			/* Caso contrário:
			Verificar qual o comando;
			Preparar msg_out;
			Usar network_send_receive para enviar msg_out para
			o server e receber msg_resposta.
			*/
		}else{
				if(strcmp(aux,"put\0")==0){

					rtable->actTable = atoi(strtok(NULL," "));
					char *key=strdup(strtok(NULL," "));
					char *aux= strtok(NULL,"\0");
					struct data_t *value=data_create4(strlen(aux)-1,aux);

					while (rtables_put(rtable,key,value)==-1){
						if (temp <= 1) {
							temp++;
						}else{
							temp=0;
							if((rtables_unbind(rtable))==NULL){
								return -1;
							}
							if((rtablesec = rtables_bind(argv[2]))==NULL){
								return -1;
							}
						}
					}

				}
	 			if(strcmp(aux,"get\0")==0){

					rtable->actTable = atoi(strtok(NULL," "));
					char *key=strdup(strtok(NULL,"\n"));
					if(strcmp(key,"*")==0){
						char **ret = rtables_get_keys(rtable);
						if(ret != NULL){
						while (rtables_free_keys(ret)==NULL){
							if (temp <= 1) {
								temp++;
							}else{
								temp=0;
								if((rtables_unbind(rtable))==NULL){
									return -1;
								}
								if((rtablesec = rtables_bind(argv[2]))==NULL){
									return -1;
								}
							}
						}
						free(key);
						}else{
							free(key);
						}
					}else{
						struct data_t dest;
						while ((dest = rtables_get(rtable,key))==NULL){
							if (temp <= 1) {
								temp++;
							}else{
								temp=0;
								if((rtables_unbind(rtable))==NULL){
									return -1;
								}
								if((rtablesec = rtables_bind(argv[2]))==NULL){
									return -1;
								}
							}
						}
						data_destroy(dest);
						}

				}
				if(strcmp(aux, "update\0")==0){

					rtable->actTable = atoi(strtok(NULL," "));

					char *key=strdup(strtok(NULL," "));
					char *aux= strtok(NULL,"\0");
					struct data_t *value=data_create4(strlen(aux)-1,aux);
					while (rtables_update(rtable,key,value)==-1){
						if (temp <= 1) {
							temp++;
						}else{
							temp=0;
							if((rtables_unbind(rtable))==NULL){
								return -1;
							}
							if((rtablesec = rtables_bind(argv[2]))==NULL){
								return -1;
							}
						}
					}

				}
				if(strcmp(aux, "size\0")==0){

					rtable->actTable = atoi(strtok(NULL," "));

					while (rtables_size(rtable)==-1){
						if (temp <= 1) {
							temp++;
						}else{
							temp=0;
							if((rtables_unbind(rtable))==NULL){
								return -1;
							}
							if((rtablesec = rtables_bind(argv[2]))==NULL){
								return -1;
							}
						}
					}

				}
				if(strcmp(aux, "collisions\0")==0){

					rtable->actTable = atoi(strtok(NULL,"\0"));
					while (rtables_collisions(rtable)==-1){
						if (temp <= 1) {
							temp++;
						}else{
							temp=0;
							if((rtables_unbind(rtable))==NULL){
								return -1;
							}
							if((rtablesec = rtables_bind(argv[2]))==NULL){
								return -1;
							}
						}
					}
				}
			}
		}
		if(rtables == NULL){
			return rtables_unbind(rtable);
		}else{
			return rtables_unbind(rtablesec);
		}

}
