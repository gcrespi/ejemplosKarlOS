/*
 ============================================================================
 Name        : Ej1.c
 Author      : karlOS
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <string.h>

#define MAX_LEN_CMD  100

typedef struct {
	char isbn[14];
	double precio;
	int stock;
	char *nombre;
} t_libro;

void free_string_splits(char ** string_splits);


//-------------------------------------------------------------------------------------
void liberar_libro(t_libro *libro)
{	free(libro->nombre);
	free(libro);
}

//-------------------------------------------------------------------------------------
void leerStdin(char *leido,int maxLargo)
{
	fgets(leido,maxLargo,stdin);
	if((strlen(leido)>0) && (leido[strlen(leido)-1]=='\n'))
	{	leido[strlen(leido)-1]='\0';}

}


//-------------------------------------------------------------------------------------
void mostrar_libro(t_libro *libro)
{
	printf("-> Nombre: %s\t ",libro->nombre);
	printf("ISBN: %s\t ",libro->isbn);
	printf(" Precio: %0.2lf\t Stock: %i \n",libro->precio,libro->stock);
}


//-------------------------------------------------------------------------------------
void listarLibros(t_list *listaDeLibros)
{
	list_iterate(listaDeLibros,(void*)mostrar_libro);
	printf("Fin de Lista \n");

}

//-------------------------------------------------------------------------------------
void cargarLibro(t_list *listaDeLibros,char *parametros)
{
	t_libro* libroCargado;
	char **parametrosSeparados;

	parametrosSeparados = string_n_split(parametros,4," ");

	if(strlen(parametrosSeparados[0])>13)
	{	printf("Error ISBN Demasiado Largo!!!\n");}
	else
	{
		libroCargado = malloc(sizeof(t_libro));

		strcpy(libroCargado->isbn,parametrosSeparados[0]);
		libroCargado->precio = atof(parametrosSeparados[1]);
		libroCargado->stock = atoi(parametrosSeparados[2]);
		libroCargado->nombre = string_duplicate(parametrosSeparados[3]);

		list_add(listaDeLibros,libroCargado);
	}

	free_string_splits(parametrosSeparados);
}

//-------------------------------------------------------------------------------------
bool esElLibroDelNombreBuscado(t_libro *libro,char *nombreBuscado)
{	return (strcmp(nombreBuscado,libro->nombre)==0);}


//-------------------------------------------------------------------------------------
void quitarLibro(t_list *listaDeLibros,char *nombreLibro)
{
	bool _esElLibroABorrar(t_libro *libro)
	{	return esElLibroDelNombreBuscado(libro,nombreLibro);}

	list_remove_and_destroy_by_condition(listaDeLibros,(void*) _esElLibroABorrar,(void*)liberar_libro);
}

//-------------------------------------------------------------------------------------
void cambiarStock(t_list *listaDeLibros,char *parametros,int factor)
{
	t_libro* libroAModificar;
	char **parametrosSeparados;

	int cantidad;
	char *nombre;

	parametrosSeparados = string_n_split(parametros,2," ");

	cantidad= atoi(parametrosSeparados[0]);
	nombre= parametrosSeparados[1];

	bool _esElLibroAModificar(t_libro *libro)
	{	return esElLibroDelNombreBuscado(libro,nombre);}

	libroAModificar= list_find(listaDeLibros,(void*)_esElLibroAModificar);

	libroAModificar->stock += factor*cantidad;

	free_string_splits(parametrosSeparados);
}

//-------------------------------------------------------------------------------------
void aumentarStock(t_list *listaDeLibros,char *parametros)
{	cambiarStock(listaDeLibros,parametros,1);}


//-------------------------------------------------------------------------------------
void disminuirStock(t_list *listaDeLibros,char *parametros)
{	cambiarStock(listaDeLibros,parametros,-1);}


//-------------------------------------------------------------------------------------
void free_string_splits(char ** string_splits)
{
	char **aux_splits=string_splits;

	while(*aux_splits!=NULL)
	{
		free(*aux_splits);
		aux_splits++;
	}

	free(string_splits);
}

//-------------------------------------------------------------------------------------
void realizar_comando(t_list *listaDeLibros,char *comando)
{
	char **comandoSeparado;

	comandoSeparado=string_n_split(comando,2," ");
	string_to_upper(comandoSeparado[0]);

	if(strcmp(comandoSeparado[0],"CARGARLIBRO")==0)
	{	cargarLibro(listaDeLibros,comandoSeparado[1]);}
	else
	{	if(strcmp(comandoSeparado[0],"LISTARLIBROS")==0)
		{	listarLibros(listaDeLibros);}
		else
		{	if(strcmp(comandoSeparado[0],"QUITARLIBRO")==0)
			{	quitarLibro(listaDeLibros,comandoSeparado[1]);}
			else
			{	if(strcmp(comandoSeparado[0],"AUMENTARSTOCK")==0)
				{	aumentarStock(listaDeLibros,comandoSeparado[1]);}
				else
				{	if(strcmp(comandoSeparado[0],"DISMINUIRSTOCK")==0)
					{	disminuirStock(listaDeLibros,comandoSeparado[1]);}
					else
					{	if(strcmp(comandoSeparado[0],"SALIR")!=0)
						{	printf("Comando Invalido\n");}
					}
				}
			}
		}
	}

	free_string_splits(comandoSeparado);
}


//-------------------------------------------------------------------------------------
int main(void) {

	t_list *listaDeLibros;

	char comando[MAX_LEN_CMD+1]="";

	printf("Bienvenido a la libreria de libros:\n");

	listaDeLibros=list_create();

	do
	{	printf(">");
		leerStdin(comando,MAX_LEN_CMD+1);
		printf("\n");

		realizar_comando(listaDeLibros,comando);

		string_to_upper(comando);
	}while(strcmp(comando,"SALIR")!=0);

	list_destroy_and_destroy_elements(listaDeLibros,(void*)liberar_libro);

	return 0;
}
