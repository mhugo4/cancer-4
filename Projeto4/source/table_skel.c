
#include <error.h>

#include "inet.h"
#include "table-private.h"
#include "message-private.h"

struct table_t **tables;
int num_tables;

int table_skel_init(char **n_tables){
	num_tables = atoi(n_tables[0]); 
	tables = malloc(sizeof(struct table_t) * num_tables);
	if(tables == NULL){ 
		return -1;
	}
	int i = 1;
	for(; i <= num_tables;i++){
		
		tables[i] = table_create(atoi(n_tables[i]));
		if(tables[i] == NULL)
			return -1;
	}
	return 0;
}


int ntables_operation(){
	return num_tables;
}

int put_operation(struct message_t *msg_in, struct table_t *tabela){

    struct entry_t *entry = entry_dup(msg_in->content.entry);
	printf("key = %s \n", entry->key);
    int result = table_put(tabela, entry->key, entry->value);
	printf("%d \n", result);
    return result;
}

struct data_t *get_operation(struct message_t *msg_in, struct table_t *tabela){

	
    struct data_t *result = table_get(tabela, msg_in->content.key);

    return result;
}

char **get_keys_operation(struct message_t *msg_in, struct table_t *tabela){
    return table_get_keys(tabela);
}


int update_operation(struct message_t *msg_in, struct table_t *tabela){

    struct entry_t *entry = entry_dup(msg_in->content.entry);

    int result = table_update(tabela, entry->key, entry->value);
    return result;
}

int size_operation(struct message_t *msg_in, struct table_t *tabela){

    return table_size(tabela);
}

int colls_operation(struct message_t *msg_in, struct table_t *tabela){
    int colls = 0;
    int i = 0;
    for(i; i < tabela->size; i++){
	if(tabela->table_e[i].key != NULL && tabela->table_e[i].next != NULL)
	    colls++;
    }
    return colls;
}
struct message_t *invoke(struct message_t *msg_in){ 

	if(msg_in == NULL)
		return NULL;
    struct message_t *msg_resposta = malloc(sizeof(struct message_t));
    if(msg_resposta == NULL)
    	return NULL;

    short opcode = msg_in->opcode;
    short c_type = msg_in->c_type;
	printf("opcode:%d c_type:%d \n",opcode, c_type); 
    struct table_t *tabela = tables[msg_in->table_num]; 

    int result = -1;
    int error_flag = 0;
    switch(opcode){
    case OC_PUT:{
	if(c_type != CT_ENTRY){
	    error_flag = 1;
	    break;
	}
        int result = put_operation(msg_in, tabela); 
	if(result < 0){
	    error_flag = 1;
	    break;
	}
	msg_resposta->opcode = OC_PUT + 1;
	msg_resposta->c_type = CT_RESULT;
	msg_resposta->content.result = result;
	break;
    }
    case OC_GET:{
		if(c_type != CT_KEY){
		    error_flag = 1;
		    break;
		}
		if(strcmp(msg_in->content.key, "*") == 0){
			char **keys = get_keys_operation(msg_in, tabela);
			if(keys == NULL){
				error_flag = 1;
				result = -1;
			}
			msg_resposta->opcode = OC_GET + 1;
			msg_resposta->c_type = CT_KEYS;
			msg_resposta->content.keys = keys;
		}else{
			struct data_t *get_result = get_operation(msg_in, tabela);
			if(get_result == NULL){
		   		error_flag = 1;
		    	result = -1;
			}
			msg_resposta->opcode = OC_GET + 1;
			msg_resposta->c_type = CT_VALUE;
			msg_resposta->content.data = data_dup(get_result);
		}
		break;
    }
    case OC_UPDATE:{
	if(c_type != CT_ENTRY){
	    error_flag = 1;
	    break;
	}
	result = update_operation(msg_in, tabela);
	if(result < 0)
	    error_flag = 1;

	msg_resposta -> opcode = OC_UPDATE + 1;
	msg_resposta -> c_type = CT_RESULT;
	msg_resposta->content.result = result;
	break;
    }  
    case OC_SIZE:{
	result = size_operation(msg_in, tabela);
	if(result < 0)
	    error_flag = 1;
	msg_resposta -> opcode = OC_SIZE + 1;
	msg_resposta -> c_type = CT_RESULT;
	msg_resposta->content.result = result;
	
	break;
    }
    case OC_COLLS:{
	result = colls_operation(msg_in, tabela);
	if(result < 0)
	    error_flag = 1;
	msg_resposta -> opcode = OC_COLLS + 1;
	msg_resposta -> c_type = CT_RESULT;
	msg_resposta->content.result = result;
	break;
	    }
	case OC_NTABLES:{
	result = ntables_operation();
	if(result < 1)
		error_flag = 1;
	msg_resposta -> opcode = OC_NTABLES + 1;
	msg_resposta -> c_type = CT_RESULT;
	msg_resposta->content.result = result;
	break;
	}
    default:
        error_flag = 1;
    }
    if(error_flag){
	msg_resposta->opcode = 99; //Codigo de erro
	msg_resposta ->c_type = CT_RESULT;
	msg_resposta->content.result = result;
    }

    return msg_resposta;
}




int table_skel_destroy(){
	int i = 0;
	for(;i < num_tables;i++){
		table_destroy(tables[i]);
	}
	free(tables);
}


