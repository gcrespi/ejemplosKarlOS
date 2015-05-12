/*
 ============================================================================
 Name        : callProgramStd.c
 Author      : karlOS
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void sigpipe_f(int n){
	printf("dio sigpipe\n");
}


//-------------------------------------------------------------------------------------------------
void escribirBoludecesEnArchivo(FILE *archivo)
{

	while(1)
	{
		int salida;
		salida= fprintf (archivo, "boludeces1\nboludeces2\nsalir\nsalir\n");

		if(salida<0){
			printf("Error in fprintf\n");
			return;
		}
	}

}


//-------------------------------------------------------------------------------------------------
int llamada_al_programa_redireccionando_stdin_out_ordenando(char *pathPrograma,char *pathArchivoSalida,void (*escribirArchivoEntrada) (void*))
{
	FILE *entradaARedirigir = NULL;

	char *comandoEntero= malloc(strlen(pathPrograma)+11+strlen(pathArchivoSalida));


	sprintf(comandoEntero,"%s | sort > %s",pathPrograma,pathArchivoSalida);

	entradaARedirigir = popen (comandoEntero,"w");

	if (entradaARedirigir != NULL)
	{
		printf("empece\n");
		escribirArchivoEntrada(entradaARedirigir);
		printf("termine\n");
		pclose (entradaARedirigir);

		free(comandoEntero);
	}
	else
	{
		printf("No se pudo ejecutar el programa!");
		return -1;
	}


	return 0;
}


//-------------------------------------------------------------------------------------------------
int main(void) {
	signal(SIGPIPE, sigpipe_f);

	puts("Hola Soy un programa que redirecciona el stdin y el stdout para un cierto programa!!!\n");

	llamada_al_programa_redireccionando_stdin_out_ordenando("/home/utnso/git/ejemplosKarlOS/Ej1/Debug/Ej1","salida.txt",(void *) escribirBoludecesEnArchivo);

	return EXIT_SUCCESS;
}
