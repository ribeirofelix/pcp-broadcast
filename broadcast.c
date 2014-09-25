
#include "broadcast.h"
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>

#define TAM 10

/* variáveis globais - vou supor um buffer circular */

int buffer[TAM];

int proximaEscrita = 0;

int quantosLeram[TAM]; /* indica quantas threads leram uma posição */
int receptoresEsperando[TAM];
int *marcadorLeitura[TAM];

int totalReceptores = 0 ;
int totalTransmissores = 0 ;


int * correntes ; /* posição de leitura de cada thread destinatário */



sem_t cons[TAM];
sem_t prod;
int dc = 0;
int dp = 0 ;
sem_t e;

void incrementaCircular(int *);

int inicia (int transmissores, int receptores)
{
	int i ;
	printf("vai\n");
	correntes = (int*) malloc(sizeof(int)*receptores);
	if (correntes == NULL)
		return 0;

	for ( i = 0; i < receptores; ++i)
		correntes[i] = 0 ;

	for (i = 0; i < TAM; ++i)
	{
		buffer[i] = 0;
		quantosLeram[i] = 0 ;
		receptoresEsperando[i] = 0;
		marcadorLeitura[i] = (int*) malloc(sizeof(int)*receptores);
		sem_init(&cons[i],0,0);
	}


	totalReceptores = receptores;
	totalTransmissores = transmissores;
	
	sem_init(&prod,0,0);
	sem_init(&e,0,1);

	return 1;
}

void envia (int val)
{
	int i;
	/* <await (B)S; > */
	/* B =  quantosLeram[proximaEscrita] == totalReceptores       */
	/* S = deposita item 										 */
	/* <await (deposita[proximaEscrita] == totalReceptores ) >    */

	//P(e)
	sem_wait(&e);
	
	// if (!B)
	if (  ! quantosLeram[proximaEscrita] == totalReceptores )
	{
		//dp++
		dp++;
		//V(e)
		sem_post(&e);
		printf("wait prod - envia \n");
		//P(prod)
		sem_wait(&prod);
		printf("saiu wait prod - envia\n");
	}

	printf("antes coloca\n");

	//S
	buffer[proximaEscrita] = val;
	proximaEscrita++;

	for (i = 0; i < totalReceptores; ++i)
	{
		marcadorLeitura[proximaEscrita-1][i] = 0;
	}

	printf("depois coloca\n");

	//SIGNAL
	if (dp > 0 && quantosLeram[proximaEscrita-1]  == totalReceptores ) 
	{
		dp--;
		printf("post prod\n");
		sem_post(&prod);
	}else if ( receptoresEsperando[proximaEscrita] > 0 )
	{
		printf(" post cons[corrrente] \n");
		receptoresEsperando[proximaEscrita]--;
		sem_post(&cons[proximaEscrita]);
	}else
		sem_post(&e);

}

int recebe (int meu_id)
{
	int corrente = correntes[meu_id];
	int item; 
	/* <await (B)S; > */
	/* B =  proxEscrita > corrente && quantosLeram[corrente] != totalReceptores       */
	/* S = pegar item 																  */
	/* <await (proxEscrita > corrente && quantosLeram[corrente] != totalReceptores ) >*/

	//P(e)
	printf("wait e\n");
	sem_wait(&e);
	printf("saiu e\n");
	
	// if (!B)
	if ( marcadorLeitura[corrente][meu_id] )
	{
		//db ++
		receptoresEsperando[corrente]++;
		sem_post(&e);
		//V(e)
		printf("post e \n");
		//P(sb)
		sem_wait(&cons[corrente]);
		printf("wait cons\n");
	}


	//S:
	printf("critica %d , id %d \n", buffer[corrente] , meu_id);

	item = buffer[corrente];
	printf("%d\n",item );	
	
	correntes[meu_id] = (corrente + 1) % TAM;
	marcadorLeitura[corrente][meu_id] = 1;
	

	//SIGNAL
	if (receptoresEsperando[corrente] > 0 && !marcadorLeitura[corrente][meu_id] )
	{
		receptoresEsperando[corrente]--;
		sem_post(&cons[corrente]);
	}else if (dp > 0 )
	{
		dp--;
		printf("post prod em recebe\n");
		sem_post(&prod);
	}
	else
		sem_post(&e);


	
	

	// <quantosLeram[corrente]++>
	printf("oi1?\n");
	sem_wait(&e);
	printf("oi?\n");

	quantosLeram[corrente]++;

	//SIGNAL
	if (dp > 0  && proximaEscrita == corrente && quantosLeram[corrente] == totalReceptores )
	{
		dp--;
		sem_post(&prod);
	}
	else if ( receptoresEsperando[corrente] > 0 )
	{
		receptoresEsperando[corrente] --;
		sem_post(&cons[corrente]);
		/* code */
	}
	else
		sem_post(&e);


	return item;
}


void incrementaCircular(int * inc)
{
	proximaEscrita = (++(*inc)) % TAM; 
}