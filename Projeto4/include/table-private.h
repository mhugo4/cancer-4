//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34
#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H
#include "entry.h"


struct table_t {
	int numPares; /* Numero de pares*/
	int size; /*O tamanho da tabela*/
	struct entry_t *entrada;/* Pointer para a tabela*/
	int posLivre; //eh  a posicao livre mais alta na tabela
	int numCols;//numero de colisoes na tabela
};

/*
Funcao que calcula o hash
*/
int hashe(char *ptr ,int size);

struct entry_t *procuraEntrada(struct table_t *table,char * key, struct entry_t ** corrente);

#endif
