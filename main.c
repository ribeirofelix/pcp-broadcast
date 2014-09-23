#include "broadcast.h"
#include <pthread.h>
#include <stdio.h>

void *w1()
{
	
	printf("antes envia\n");
	envia(3);
	return NULL;
}

void * w2(void *x_void_ptr)
{
	int * val_ptr = (int*)x_void_ptr;
	printf("antes recebe\n");
	*val_ptr = recebe(0);
	printf("pos recebe\n");
	return NULL;
}

int main()
{
	int val;
	printf("antes inicia\n");
	inicia(1,1);


	pthread_t inc_x_thread;

	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&inc_x_thread, NULL, w2, &val )) {

		printf("Error creating thread\n");
		return 1;
	}


	printf("antes envia\n");
	envia(3);

	
	return 0;
}