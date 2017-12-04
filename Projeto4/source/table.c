
//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34
#include <stdio.h>
#include "entry.h"
#include <string.h>
#include "data.h"
#include "table.h"
#include "entry.h"
#include "table-private.h"


/* Função para criar/inicializar uma nova tabela hash, com n
 * linhas(n = módulo da função hash)
 */
struct table_t *table_create(int n){

	if(n <=0){
		return NULL;
	}

	struct table_t *tabela=(struct table_t *)malloc(sizeof(struct table_t));
	if (tabela == NULL) {
		return NULL;
	}

	tabela->size = n;
	tabela->numPares = 0;
	tabela->numCols = 0;
	tabela->entrada = (struct entry_t *) malloc(n * sizeof(struct entry_t));
	//temos de inicializar a entrada com metodo entry_initialize
	if(tabela->entrada==NULL){
		free(tabela);
		return NULL;
	}

	int i;
	for (i = 0; i< n ;i++) {
		entry_initialize(&(tabela->entrada[i]));
	}

	tabela->posLivre = n-1;
	return tabela;
}



/* Libertar toda a memória ocupada por uma tabela.
 */
void table_destroy(struct table_t *table){
	if(table != NULL){
		int i;
		for (i=0; i < table->size; ++i)
		{
			data_destroy(table->entrada[i].value);
			free(table->entrada[i].key);

		}

		free(table->entrada);
		free(table);
	}else{
		return;
	}
}

/* Função para adicionar um par chave-valor na tabela.
 * Os dados de entrada desta função deverão ser copiados.
 * Devolve 0 (ok) ou -1 (out of memory, outros erros)
 */
int table_put(struct table_t *table, char *key, struct data_t *value){

	if(table == NULL || key == NULL || value == NULL){

		return -1;
	}

	if(table->numPares == table->size){

		return -1;
	}
	int i;
	for (i = 0; i < table->size; i++)
	{
		struct entry_t *comp = &(table->entrada[i]);

		if(comp->key != NULL){
			if(strcmp(comp->key,key)== 0){
				return -1;

			}

		}
	}
	int hash = hashe(key,table->size);

	struct entry_t *comp = &(table->entrada[hash]);


	if(hash == table->posLivre){//se o hash for igual ah ultima posicao da tabela
		table->posLivre--;
	}


	if(comp->key== NULL){
		comp->key=strdup(key);
		comp->value=data_dup(value);
		table->numPares++;

		return 0;

	}else{
		if(table->posLivre==-1){

			return -1;

		}
		if(strcmp(comp->key,key)==0){


			return -1;// -1 e nao zero porque isto nao eh o update
		}
		comp=procuraEntrada(table, key, &(comp));
		comp->key=strdup(key);
		comp->value=data_dup(value);
		table->numPares++;
		table->numCols++;

		return 0;


	}

	return -1;

}


/* Função para substituir na tabela, o valor associado à chave key.
 * Os dados de entrada desta função deverão ser copiados.
 * Devolve 0 (OK) ou -1 (out of memory, outros erros)
 */
int table_update(struct table_t *table, char *key, struct data_t *value){


	if(key == NULL|| table ==NULL || value == NULL){


		return -1;
	}else{

		int hash = hashe(key,table->size);

		struct entry_t *comp = &(table->entrada[hash]);

		if(comp!= NULL){
			int i ;

			for (i = 0; i < table->size; i++)
			{
				struct entry_t *comp = &(table->entrada[i]);

				if(comp->key != NULL){

					if(strcmp(comp->key,key)== 0){

						data_destroy(comp->value);
						comp->key=strcpy(comp->key,key);
						comp->value = data_dup(value);
						return 0;
					}
				}
			}
		}
		return -1;
	}
}


/* Função para obter da tabela o valor associado à chave key.
 * A função deve devolver uma cópia dos dados que terão de ser libertados
 * no contexto da função que chamou table_get.
 * Devolve NULL em caso de erro.
 */
struct data_t *table_get(struct table_t *table, char *key){

	if(key == NULL|| table ==NULL){

		return NULL;
	}else{


		int hash = hashe(key,table->size);

		struct entry_t *comp = &(table->entrada[hash]);

		if(comp!= NULL){
			int i ;

			for (i = 0; i < table->size; i++)
			{
				struct entry_t *comp = &(table->entrada[i]);

				if(comp->key != NULL){

					if(strcmp(comp->key,key)== 0){

						return data_dup(comp->value);
					}
				}
			}
		}

		return NULL;
	}

}

/* Devolve o número de elementos na tabela.
 */
int table_size(struct table_t *table){
	if(table==NULL){
		return 0;
	}
	return table->numPares;
}

/* Devolve um array de char * com a cópia de todas as keys da tabela,
 * e um último elemento a NULL.
 */
char **table_get_keys(struct table_t *table){
	if(table==NULL){
		return NULL;
	}
	int i;
	char **ret=malloc(table_size(table)*sizeof(char *));
	for(i = 0; i <= table->size-1;i++){
		struct entry_t *comp =&(table->entrada[i]);
		if(comp!=NULL){
			if(comp->key!=NULL){
				ret[i] = strdup(comp->key);
			}
		}
		if(table->size== i){
			ret[(table->size)]= NULL;
		}
	}
	return ret;
}

/* Liberta a memória alocada por table_get_keys().
 */
void table_free_keys(char **keys){
	if(keys[0]==NULL){
		return;
	}
	int i;

	for (i = 0; keys[i] != NULL; i++)
	{
		free(keys[i]);
	}
	free(keys);
}
int table_colls(struct table_t *table){
	return table->numCols;
}
