//Hugo Garcia nº 47800 grupo 34
//Dinis Silva nº47881 grupo 34
//Nuno Torcato nº47884 grupo 34
#ifndef _MESSAGE_PRIVATE_H
#define _MESSAGE_PRIVATE_H
#include "message.h"
#define OC_RT_ERROR 99
/**
* Assegura que ao receber um array de bytes o escreve na totalidade  mesmo que a mensagen
* venha fragmentada
*/
int write_all(int sock, char *buf, int len);

/**
* Assegura que ao receber um array de bytes o le na totalidade  mesmo que a mensagen
* venha fragmentada
*/
int read_all(int sock, char *buf, int len);


#endif
