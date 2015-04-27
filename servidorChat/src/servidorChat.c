/*
 ============================================================================
 Name        : servidorChat.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MYPORT 5552
#define BACKLOG 7

void escucharInvitadosChat()
{
	int socketEscucha,socketConeccion,len;
	struct sockaddr_in miDireccion;
	struct sockaddr_in clienteDireccion;
	int sin_size;
	char mensaje[50], aux[50];

	socketEscucha= socket(AF_INET,SOCK_STREAM,0);
	miDireccion.sin_family=AF_INET;
	miDireccion.sin_port= htons(MYPORT);
	miDireccion.sin_addr.s_addr= inet_addr("127.0.0.1");
//	miDireccion.sin_addr.s_addr= INADDR_ANY;

	memset(&(miDireccion.sin_zero),'\0',8);

	bind(socketEscucha, (struct sockaddr *) &miDireccion, sizeof(struct sockaddr));

	printf("Por escuchar\n");

	listen(socketEscucha,BACKLOG);

	printf("Escuchando...\n");

	sin_size = sizeof(struct sockaddr_in);

	socketConeccion = accept(socketEscucha, (struct sockaddr *)&clienteDireccion,(socklen_t*) &sin_size);

	printf("Conectado con una compu\n");


	while(1){
	recv(socketConeccion,mensaje,4,0);
	len=atoi(mensaje);

	recv(socketConeccion,mensaje,len,0);
	printf("Envio: %s\n",mensaje);

	fgets(mensaje,strlen(mensaje)+1,stdin);
	len= strlen(mensaje)+1;
	sprintf(mensaje,"%i",len);
	send(socketConeccion,aux,4,0);
	send(socketConeccion,mensaje,len,0);

	}

	printf("Cerrando Socket\n");
	close(socketConeccion);
	close(socketEscucha);

	printf("Socket Cerrado\n");

}


//void crear_hilo_escuchador()
//{
//	pthread_t hiloEscucha;
//
//	pthread_create(&hiloEscucha,NULL,(void *)escucharInvitadosChat,NULL);
//
//	pthread_join(hiloEscucha,NULL);
//
//
//}


int main(void) {

	printf("Bienvenido al Chat!\n");

	escucharInvitadosChat();
//	crear_hilo_escuchador();


	return EXIT_SUCCESS;
}
