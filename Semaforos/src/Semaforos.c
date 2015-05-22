/*
 ============================================================================
 Name        : Semaforos.c
 Author      : Franco
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
//Ejemplo para sincronizar con semaforos elementos de una lista compartida entre 2 hilos.


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <cspecs/cspec.h>

//definición de la estructura y_persona-------------------------------------------------------------------------------------------
typedef struct {
    char *nombre;
    int edad;
    double dni;
} t_persona;

//variables globales como semáforos, hilos, y contadores inicializados------------------------------------------------------------
sem_t semaforo1;
sem_t semaforo2;
int num=10;
pthread_t thread1, thread2, thread3;
/*t_persona *gente[20];
t_list *list;*/


//encabezados de las funciones utilizadas----------------------------------------------------------------------------------------
t_persona *persona_create(char *nombre, int edad, double dni);
void persona_destroy();
//void funcionUno(int *valor);
//void funcionDos(int *valor);
void funcionDeMuestra(t_list *lista);
void funcionDeFiltrado(t_list *lista);
void liberarLista(t_list *lista);



//definición de las funciones-----------------------------------------------------------------------------------------------------


//Funcion para crear un elemento de tipo t_persona de la lista--------------------------------------------------------------------
t_persona *persona_create(char *nombre, int edad, double dni){
	t_persona *nuevo = malloc(sizeof(t_persona));
	nuevo->nombre = nombre;
	nuevo->edad = edad;
	nuevo->dni = dni;
	return nuevo;
}


//Función para eliminar un elemento de la lista-----------------------------------------------------------------------------------
void persona_destroy(t_persona *self){

	free(self);
	free(self->nombre);
}


//función uno---------------------------------------------------------------------------------------------------------------------
/*void funcionUno(int *valor){
	while(1){
		sem_wait(&semaforo1);
	    printf("variable vale %d \n",(*valor));
		sem_post(&semaforo2);
	}
}
*/

/*Función Dos---------------------------------------------------------------------------------------------------------------------
void funcionDos(int *valor){
	while(1){
		sem_wait(&semaforo2);
		(*valor)++;
		printf("variable ahora vale %d \n",(*valor));
		sem_post(&semaforo1);
	}
}
*/

//Función de muestreo de datos-------------------------------------------------------------------------------------------------------------------
void muestraDePersonas(t_persona *personas){
	printf("-> Nombre: %s\t ",personas->nombre);
	printf("edad: %d\t ",personas->edad);
	//printf(" Precio: %d\t \n",personas->dni);
}


//Funcion de muestra iterativamente--------------------------------------------------------------------------------------------------------------
void funcionDeMuestra(t_list *lista){
	while(1){
		sem_wait(&semaforo1);
		list_iterate(lista, (void*)muestraDePersonas);
		printf("Fin Lista de personas!");
		sem_post(&semaforo2);
	}
}

//Función para liberar lista de personas---------------------------------------------------------------------------------------------------------
void liberarLista(t_list *lista){
	list_iterate(lista, (void*)persona_destroy);
	printf("Fin borrado de lista");
}
//---------------------------------

bool _Es_MenorOIgual_a_22(t_persona* persona) {
	return persona->edad <= 22;
}

//Funcion para filtrar por condición-------------------------------------------------------------------------------------------------------------
void funcionDeFiltrado(t_list *lista){
	while(1){
		sem_wait(&semaforo2);
		t_list *filtrados = list_filter(lista, (void*) _Es_MenorOIgual_a_22);
		printf("Los menores a 22 años son: ");
		funcionDeMuestra(filtrados);
		printf("fin de muestra de filtrados por edad");
		sem_post(&semaforo1);
		free(filtrados);
	}
}


//-----------------------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
	t_list *listPersonas;
	listPersonas = list_create();

	list_add(listPersonas, persona_create("Matias", 24, 38945123));
	list_add(listPersonas, persona_create("Franco", 21, 37945783));
	list_add(listPersonas, persona_create("Daniel", 22, 38911141));
	list_add(listPersonas, persona_create("Leandro",21, 37745123));
	list_add(listPersonas, persona_create("Paco",   16, 38945120));
	list_add(listPersonas, persona_create("Gaston", 25, 33946703));
	list_add(listPersonas, persona_create("Gustavo",21, 37983423));
	list_add(listPersonas, persona_create("Santiago",20,38001124));
	list_add(listPersonas, persona_create("Damian", 22, 36645550));
	list_add(listPersonas, persona_create("Pedro", 40,  32045104));
	list_add(listPersonas, persona_create("Nathan", 38, 32116132));

	if((sem_init(&semaforo1, 0, 1))==-1){
		perror("semaphore");
		exit(1);
	}
	if((sem_init(&semaforo2, 0, 0))==-1){
		perror("semaphore");
		exit(1);
	}


	if ((pthread_create( &thread1, NULL,(void *)funcionDeMuestra, &listPersonas))== -1){
		perror("thread 1");
		exit(1);
	}

	if ((pthread_create( &thread2, NULL, (void *)funcionDeFiltrado, &listPersonas))== -1){
		perror("thread 2");
		exit(1);
	}

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);

	if((pthread_create(&thread3, NULL, (void*)liberarLista, &listPersonas))==-1){
		perror("thread 3");
		exit(1);
	}

	pthread_join(thread3, NULL);


	return 0;

}
