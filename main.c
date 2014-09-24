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
	int item ;
	printf("antes recebe\n");
	item = recebe(0);
	printf("pos recebe: %d \n",  item);

	return NULL;
}

int main()
{
	int val = 0;
	int val1 = 1;

	printf("antes inicia\n");
	inicia(1,1);


	pthread_t inc_x_thread;
	pthread_t inc_y_thread;

	

	
	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&inc_x_thread, NULL, w2, &val )) {

		printf("Error creating thread\n");
		return 1;
	}
	w1();
	
	// /* create a second thread which executes inc_x(&x) */
	// if(pthread_create(&inc_y_thread, NULL, w2, &val1 )) {

	// 	printf("Error creating thread\n");
	// 	return 1;
	// }
	



	
	return 0;
}