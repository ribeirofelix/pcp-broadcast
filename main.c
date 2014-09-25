#include "broadcast.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define QTD_REPECTORAS 2
#define QTD_TRANSMISSORAS 2

pthread_t t_receptoras[QTD_REPECTORAS];
pthread_t t_transmissoras[QTD_TRANSMISSORAS];
void *respostas[QTD_REPECTORAS];

void *transmitir(void *val)
{
	int * val_ptr ;
	int v ;
	printf("comecou transmitir\n");

	val_ptr  = (int*)val;
	v = (*val_ptr) +1;
	printf("antes envia:%d \n",v);
	envia( v );
	return NULL;
}

void * receber(void *id_ptr)
{
	int id = *((int*)id_ptr);
	int * item = (int*) malloc(sizeof(int));
	printf("antes recebe\n");
	*item = recebe(id);
	printf("pos recebe: %d \n",  *item);

	return item;
}


int main()
{

	int i ;
	printf("antes inicia\n");
	inicia(QTD_TRANSMISSORAS,QTD_REPECTORAS);
	
	printf("lanca_receptoras\n");
		for (i = 0; i < QTD_REPECTORAS ; ++i)
	{

		if(pthread_create(& (t_receptoras[i]), NULL, receber, &i )) {

			printf("Error creating thread\n");
			return ;
		}
	}


	printf("lanca_transmissoras\n");
	for (i = 0; i < QTD_TRANSMISSORAS; ++i)
	{
		printf("loop\n");
		if(pthread_create(& (t_transmissoras[i]), NULL, transmitir, &i )) {

			printf("Error creating thread\n");
			return ;
		}
	}
	
	for (i = 0; i < QTD_TRANSMISSORAS; ++i)
	{
		pthread_join(  (t_transmissoras[i]), NULL );
	}

	for (i = 0; i < QTD_REPECTORAS; ++i)
	{
		pthread_join(  (t_receptoras[i]), &respostas[i] );
	}

	printf("respostas:\n");
	for (i = 0; i < QTD_REPECTORAS; ++i)
	{
		printf("%d\n", *((int*)respostas[i]) );
	}
	
	return 0;
}