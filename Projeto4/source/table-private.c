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


int hashe(char *ptr ,int size){
	if(ptr ==NULL || size <=0 ){
		return NULL;
	}
	int x =strlen(ptr);

	if( x > 4){
		return ((int)ptr[0]+(int)ptr[1]+(int)ptr[x-1]+(int)ptr[x-2])% size;
	}else{
		int i;
		int sum = 0;
		for(i=0;i<x; i++){
			sum += (int)ptr[i];
		}
		return sum % size;
	}

}

struct entry_t *procuraEntrada(struct table_t *table,char * key, struct entry_t ** corrente){
	if(*corrente == NULL){
		int i;
		for (i = table->size-1; i >= 0 ; i--)
		{
			if(table->entrada[i].key==NULL){
						table->posLivre=i;
						break;
			}	
		}
		if(table->posLivre==-1){
			return NULL;
		}
		*corrente=&(table->entrada[table->posLivre]);
		return *corrente;
	}else{
		return procuraEntrada(table,key,&((*corrente)->next));
	}
}