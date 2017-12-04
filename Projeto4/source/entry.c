//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34

#include <stdio.h>
#include "entry.h"
#include <string.h>
#include "data.h"


/* Função que inicializa os membros de uma entrada na tabela com
   o valor NULL.
 */
void entry_initialize(struct entry_t *entry){
	if(entry != NULL){
		entry->key=NULL;
		entry->value=NULL;
		entry->next=NULL;
	}
}

/* Função que duplica um par {chave, valor}.
 */
struct entry_t *entry_dup(struct entry_t *entry){
	
	if(entry== NULL){
		return NULL;
	}else{

	struct entry_t *entrada = (struct entry_t *) malloc(sizeof(entry));
	entry_initialize(entrada);
	entrada->key=strdup(entry->key);
	entrada->value=data_dup(entry->value);
	entrada->next=entry->next;

	return entrada;
	}
}
