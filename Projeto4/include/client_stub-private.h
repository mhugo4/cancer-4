#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include "data.h"

/* Remote table. A definir pelo grupo em client_stub-private.h
 */
struct rtables_t{

	int  nTables; // numero de tabelas do servidor
	struct server_t *server; // server
	int actTable; //tabela ativa
	

};

#endif
