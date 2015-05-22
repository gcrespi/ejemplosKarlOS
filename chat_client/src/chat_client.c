/*
 ============================================================================
 Name        : chat_client.c
 Author      : karlOS
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//Defines
#define MAX_MSG 100	//maximo largo de un mensaje
#define DEST_PORT 3490 //puerto de destino
#define SRC_IP "127.0.0.1" //Ip de destino

//Prototypes

//-------------------------------------------------------------------------------------
void leerStdin(char *leido, int maxLargo) {
	fgets(leido, maxLargo, stdin);
	if ((strlen(leido) > 0) && (leido[strlen(leido) - 1] == '\n')) {
		leido[strlen(leido) - 1] = '\0';
	}
}



//-------------------------------------------------------------------------------------
int main(void) {

	struct sockaddr_in socketaddr_srv; //estructura con puerto y direccion ip de Destino

	int socketfd_srv;	//file descriptor del Socket
	char msg[MAX_MSG + 1]; //mensaje que voy a mandar/recibir
	int len_msg;	//largo del mensaje

	socketaddr_srv.sin_family = AF_INET;//familia de direcciones (siempre AF_INET)
	socketaddr_srv.sin_port = htons(DEST_PORT);	//setea Puerto a conectarme
	socketaddr_srv.sin_addr.s_addr = inet_addr(SRC_IP); //Setea Ip a conectarme
	memset(&socketaddr_srv.sin_zero, '\0', 8); //pone en ceros los bits que sobran de la estructura

	socketfd_srv = socket(AF_INET, SOCK_STREAM, 0);	//Crea un Socket FIXME FALTA CHECKEAR ERRORES!!!
	connect(socketfd_srv, (struct sockaddr*) &socketaddr_srv,
			sizeof(struct sockaddr));//conecta con el servidor FIXME FALTA CHECKEAR ERRORES!!!

	len_msg = recv(socketfd_srv, msg, MAX_MSG + 1, 0);//recive del servidor FIXME FALTA CHECKEAR ERRORES!!!
	msg[len_msg] = '\0'; //XXX por que?

	if (len_msg != 40) { //XXX what?
		do {
			printf("%s dice: %s\n", inet_ntoa(socketaddr_srv.sin_addr), msg);
			printf("Tu: ");
			leerStdin(msg, MAX_MSG + 1);
			send(socketfd_srv, msg, strlen(msg), 0); //envia mensaje escrito al servidor FIXME FALTA CHECKEAR ERRORES!!!

			len_msg = recv(socketfd_srv, msg, MAX_MSG + 1, 0); //recive una respuesta FIXME FALTA CHECKEAR ERRORES!!!
			msg[len_msg] = '\0';
		} while (len_msg != 0 && msg[0] != '\0'); //repetir hasta que el mensaje sea palabra vacia
		if (len_msg == 0)
			puts("El servidor se ha desconectado");
	} else {
		printf("%s dice: %s\n", inet_ntoa(socketaddr_srv.sin_addr), msg);
	}

	return EXIT_SUCCESS;
}
