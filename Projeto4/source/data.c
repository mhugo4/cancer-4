//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34

#include <stdio.h>
#include <string.h>
#include "data.h"



/* Função que cria um novo elemento de dados data_t e reserva a memória
 * necessária, especificada pelo parâmetro size 
 */

struct data_t *data_create(int size){

	if(size < 0){
		return NULL;
	}else{
	struct data_t *data = (struct data_t*)malloc(sizeof(struct data_t));
	if(data == NULL){
		return NULL;
	}
	data->datasize = size;
	data->data= malloc(sizeof(size));
	if (data->data == NULL)
	{

		free(data);
		return NULL;
	}
	return data;
	}
}




/* Função idêntica à anterior, mas que inicializa os dados de acordo com
 * o parâmetro data.
 */
struct data_t *data_create2(int size, void *data){
	
	if(size <= 0 || data == NULL ){
		return NULL;
	}else{
		struct data_t *d = data_create(size);
	    memcpy(d->data,data,size);
   		return d;
   	}
}

struct data_t *data_create3(int size){
	
	if(size <= 0){
		return NULL;
	}else{
	struct data_t *data = (struct data_t*)malloc(sizeof(struct data_t));
	if(data == NULL){
		return NULL;
	}
	data->datasize = size;
	data->data= malloc(sizeof(size));
	if (data->data == NULL)
	{

		free(data);
		return NULL;
	}
	return data;
	}
}

struct data_t *data_create4(int size, void *data){
	
	if(size <= 0 || data == NULL ){
		return NULL;
	}else{
		struct data_t *d = data_create3(size);
	    memcpy(d->data,data,size);
   		return d;
   	}
}




/* Função que destrói um bloco de dados e liberta toda a memória.
 */
void data_destroy(struct data_t *data){
	if(data != NULL){
	
	free(data->data);
	free(data);
	}
}

/* Função que duplica uma estrutura data_t.
 */
struct data_t *data_dup(struct data_t *data){
	if(data == NULL){
		return NULL;
	}else{
		struct data_t *duplicada= data_create4 (data->datasize,data->data);
		if(duplicada  == NULL){
			return NULL;
		}else{
			return duplicada;
		}
	}
}
