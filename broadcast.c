
#include "broadcast.h"
#include <stdlib.h>
#include <semaphore.h>

#define TAM 10

/* variáveis globais - vou supor um buffer circular */

int buffer[TAM];

int proximaEscrita;

int quantosLeram[TAM]; /* indica quantas threads leram uma posição */

int totalReceptores = 0 ;

/* variáveis por thread: */



int * correntes ; /* posição de leitura de cada thread destinatário */



sem_t cons;
sem_t prod;
int db;
int dp;
sem_t e;

void incrementaCircular(int *);

int inicia (int transmissores, int receptores)
{
	correntes = (int*) malloc(sizeof(int)*receptores);
	if (correntes == NULL)
		return 0;

	totalReceptores = receptores;
	
	sem_init(&cons, 0, 0);
	sem_init(&prod,0,0);
	sem_init(&e,0,1);
	
	db = dp = 0;
	
	return 1;
}

void envia (int val)
{
	sem_wait(&e);
	if ( quantosLeram[proximaEscrita] == totalReceptores )
	{
		dp++;
		sem_post(&e);
		sem_wait(&prod);
	}
	buffer[proximaEscrita] = val;

	//SIGNAL
	if (dp > 0 )
	{
		dp--;
		sem_post(&prod);
	}else if (db > 0 )
	{
		db--;
		sem_post(&cons);
	}else
		sem_post(&e);

}

int recebe (int meu_id)
{
	int corrente = correntes[meu_id];
	int item; 
	//P(e)
	sem_wait(&e);
	/* <await (proxEscrita > corrente) >*/
	if (! proximaEscrita > corrente)
	{
		db++;
		sem_post(&e);
		sem_wait(&cons);
	}
	//S:
	item = buffer[corrente];

	incrementaCircular(&corrente);
	correntes[meu_id] = corrente;

		//SIGNAL
	if (dp > 0 && corrente < proximaEscrita )
	{
		dp--;
		sem_post(&prod);
	}else if (db > 0 && corrente > 0 )
	{
		db--;
		sem_post(&cons);
	}else
		sem_post(&e);


	quantosLeram[corrente-1]++;

	return 1;
}


void incrementaCircular(int * inc)
{
	proximaEscrita = (++(*inc)) % TAM; 
}