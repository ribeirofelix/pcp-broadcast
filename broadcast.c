
#include "broadcast.h"
#include <pthread.h>
#include <semaphore.h>

#define TAM 10

/* variáveis globais - vou supor um buffer circular */

int buffer[TAM];

int proximaEscrita;

int quantosLeram[TAM]; /* indica quantas threads leram uma posição */

/* variáveis por thread: */

int corrente = 0; /* posição de leitura de cada thread destinatário */

int item; 

sem_t cons;
sem_t prod;
int db;
int dp;
sem_t e;

void incrementaCircular(int *);

int inicia (int transmissores, int receptores)
{
	sem_init(&cons, 0, 0);
	sem_init(&prod,0,0);
	sem_init(&e,0,1);
	db = dp = 0;
	return 1;
}

void envia (int val)
{
	sem_wait(&e);
	if ( proximaEscrita == corrente )
	{
		dp++;
		sem_post(&e);
		sem_wait(&prod);
	}
	buffer[corrente] = val;

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

}

int recebe (int meu_id)
{
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
	return 1;
}


void incrementaCircular(int * inc)
{
	proximaEscrita = (++(*inc)) % TAM; 
}