/*
 ============================================================================
 Name        : console.c
 Author      : karlOS
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>

//Constants
#define MAX_COMMAND_LENGTH 100
#define MAX_COMMANOS_VALIDOS 30
#define BOLD "\033[1m"
#define NORMAL "\033[0m"
#define CLEAR "\033[H\033[J"

//Prototypes
void free_string_splits (char**);
void receive_command(char*,int);
char execute_command(char*);
void help();

 //---------------------------------------------------------------------------
int main(void) {
	char command[MAX_COMMAND_LENGTH+1];
	char end;
	puts(CLEAR NORMAL"Console KarlOS\nType 'help' to show the commands");
	do
	{
		printf("> ");
		receive_command(command,MAX_COMMAND_LENGTH+1);
		end = execute_command(command);
	}while(!end);


	return EXIT_SUCCESS;
}
//---------------------------------------------------------------------------

void free_string_splits (char** strings){
	char **aux=strings;

	while(*aux != NULL){
		free(*aux);
		aux++;
	}
	free(strings);
}

//---------------------------------------------------------------------------

void receive_command(char* readed, int max_command_length){
	fgets(readed,max_command_length,stdin);
	readed[strlen(readed)-1] = '\0';
}

//---------------------------------------------------------------------------

char execute_command(char* command){

	char comandos_validos[MAX_COMMANOS_VALIDOS][16]={"help","format","pwd","cd","rm","mv","rename","mkdir","rmdir","mvdir",
													"renamedir","upload","download","md5","blocks","rmblock","cpblock","addnode","rmnode","clear",
													"exit","","","","","","","","",""};

	int i,salir=0;
	for(i=0;command[i]==' ';i++);
	if(command[i]=='\0')
	{	return 0;}

	char** subcommands = string_split(command," ");

	for(i=0;(i<MAX_COMMANOS_VALIDOS)&&(strcmp(subcommands[0],comandos_validos[i])!=0);i++);

	switch(i)
	{
		case  0: help(); break;
//		case  1: format(); break;
//		case  2: pwd(); break;
//		case  3: cd(subcommands[1]); break;
//		case  4: rm(subcommands[1]); break;
//
//		case  5: mv(subcommands[1],subcommands[2]); break;
//		case  6: renamedir(subcommands[1],subcommands[2]); break;
//		case  7: mkdir(subcommands[1]); break;
//		case  8: rmdir(subcommands[1]); break;
//		case  9: mvdir(subcommands[1],subcommands[2]); break;
//
//		case 10: renamedir(subcommands[1],subcommands[2]); break;
//		case 11: upload(subcommands[1],subcommands[2]); break;
//		case 12: download(subcommands[1],subcommands[2]); break;
//		case 13: md5(subcommands[1]); break;
//		case 14: blocks(subcommands[1]); break;
//
//		case 15: rmblock(subcommands[1],subcommands[2]); break;
//		case 16: cpblock(subcommands[1],subcommands[2],subcommands[3]); break;
//		case 17: addnode(subcommands[1]); break;
//		case 18: rmnode(subcommands[1]); break;
		case 19: printf(CLEAR); break;

		case 20: salir=1; break;

		default:
			printf("%s: no es un comando valido\n",subcommands[0]);
			break;
	}

	free_string_splits(subcommands);
	return salir;
}

//---------------------------------------------------------------------------
void help(){
	puts(BOLD" format"NORMAL" -> Formatea el MDFS.");
	puts(BOLD" pwd"NORMAL" -> Indica la ubicacion actual.");//?????
	puts(BOLD" cd (path dir)"NORMAL" -> Mueve la ubicacion actual al directorio (path dir)."); //?????
	puts(BOLD" rm (path file)"NORMAL" -> Elimina el archivo (path file).");
	puts(BOLD" mv (old path file) (new path file)"NORMAL" -> Mueve el archivo de (old path file) a (new path file).");
	puts(BOLD" rename (old path file) (nuevo path file)"NORMAL" -> Renombra el archivo (old name file) con (new name file)."); //?????
	puts(BOLD" mkdir (path dir)"NORMAL" -> Crea el directorio (path dir).");
	puts(BOLD" rmdir (path dir)"NORMAL" -> Elimina el directorio (path dir).");
	puts(BOLD" mvdir (old path dir) (new path dir)"NORMAL" -> Mueve el directorio de (old path dir) a (new path dir).");
	puts(BOLD" renamedir (old path dir) (nuevo path dir)"NORMAL" -> Renombra el directorio (old name dir) con (new name dir)."); //?????
	puts(BOLD" upload (local path file) (MDFS path file)"NORMAL" -> Copia el archivo del filesystem local (local path file) al MDFS (MDFS path file).");
	puts(BOLD" download (MDFS path file) (local path file)"NORMAL" -> Copia el archivo del MDFS (MDFS path file) al filesystem local (local path file).");
	puts(BOLD" md5 (path file)"NORMAL" -> Solicita el MD5 del archivo path (file).");
	puts(BOLD" blocks (path file)"NORMAL" -> Muestra los bloques que componen el archivo (path file).");
	puts(BOLD" rmblock (num block) (path file)"NORMAL" -> Elimina el bloque nro (num block) del archivo (path file).");
	puts(BOLD" cpblock (num block) (old path file) (new path file)"NORMAL" -> Copia el bloque nro (num block) del archivo (old path file) en el archivo (new path file).");
	puts(BOLD" addnode (node)"NORMAL" -> Agrega el nodo de datos (node).");
	puts(BOLD" rmnode (node)"NORMAL" -> Elimina el nodo de datos (node).");
	puts(BOLD" clear"NORMAL" -> Limpiar la pantalla");
	puts(BOLD" exit"NORMAL" -> Salir del MDSF");
}
