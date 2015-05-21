/*
 ============================================================================
 Name        : Semaforos.c
 Author      : Franco
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <cspecs/cspec.h>

sem_t semaforo1;
sem_t semaforo2;
int num=10;
pthread_t thread1, thread2;

void funcionUno(int *valor);
void funcionDos(int *valor);

void funcionUno(int *valor){
	while(1){
		sem_wait(&semaforo1);
	    printf("variable vale %d \n",(*valor));
		sem_post(&semaforo2);
	}
}

void funcionDos(int *valor){
	while(1){
		sem_wait(&semaforo2);
		(*valor)++;
		printf("variable ahora vale %d \n",(*valor));
		sem_post(&semaforo1);
	}
}


int main(void)
{

	if((sem_init(&semaforo1, 0, 1))==-1){
		perror("semaphore");
		exit(1);
	}
	if((sem_init(&semaforo2, 0, 0))==-1){
		perror("semaphore");
		exit(1);
	}


	if ((pthread_create( &thread1, NULL,(void *)funcionUno, &num))== -1){
		perror("pthread");
		exit(1);
	}

	if ((pthread_create( &thread2, NULL, (void *)funcionDos, &num))== -1){
		perror("pthread");
		exit(1);
	}

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);


	return 0;

}
