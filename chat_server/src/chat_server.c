/*
 ============================================================================
 Name        : chat_server.c
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

//Defines
#define BACKLOG 5
#define MAX_MSG 100

//Prototypes

int main(void) {

	fd_set master; // Nuevo set principal
	fd_set read_fds; // Set temporal para lectura
	FD_ZERO(&master); // Vacio los sets
	FD_ZERO(&read_fds);
	int fd_max; // Va a ser el maximo de todos los descriptores de archivo del select
	int i; // para los for
	int len_msg; // para la longitud de los bytes recividos
	char msg[MAX_MSG+1]; //el buffer de los mensajes

	struct sockaddr_in socketaddr, socketaddr_cli;
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = htons(3490);
	socketaddr.sin_addr.s_addr = inet_addr("192.168.0.68");
	memset(&(socketaddr.sin_zero),'\0',8);

	int listener = socket(AF_INET,SOCK_STREAM,0), socketfd_cli;
	int yes=1;
	if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	bind(listener, (struct sockaddr*) &socketaddr,sizeof(struct sockaddr));

	listen(listener, BACKLOG);
	FD_SET(listener, &master);
	fd_max = listener; //Por ahora este es el mayor

	puts("Esperando primera conexion...");

	while(1){

		read_fds = master; // Cada iteracion vuelvo a copiar del principal al temporal

		select(fd_max+1, &read_fds, NULL, NULL, NULL); // El select se encarga de poner en los temp los fds que recivieron algo

		for(i=0; i <= fd_max; i++) {
			if (FD_ISSET(i, &read_fds)) {
				if (i==listener) {
					int sin_size = sizeof(struct sockaddr_in);
					socketfd_cli = accept(listener, (struct sockaddr*) &socketaddr_cli, (socklen_t*) &sin_size); //????????
					printf("Tiene una solicitud de conexion con la IP: %s. Aceptar? s/n: ",inet_ntoa(socketaddr_cli.sin_addr));
					char caracter_valido=0, acepta[100];
					do {
						fgets(acepta,100,stdin);  //  <- ACA HAY UN BUG QUE NO SE RESOLVER
						acepta[strlen(acepta)-1] = '\0';

						if (strcmp(acepta,"n")==0 || strcmp(acepta,"N")==0) {
							send(socketfd_cli, "El servidor no ha aceptado tu solicitud.", 40, 0);
							close(socketfd_cli);
							caracter_valido=1;
						}
						else if (strcmp(acepta,"s")==0 || strcmp(acepta,"s")==0)  {
							FD_SET(socketfd_cli, &master);
							if(socketfd_cli > fd_max) fd_max = socketfd_cli;
							send(socketfd_cli, "El servidor ha aceptado tu solicitud.", 37, 0);
							caracter_valido=1;
						}
						else {
							printf("Ingrese caracter valido. Acepta? s/n: ");
						}
					} while(!caracter_valido);

				}
				else {
					len_msg = recv(i,msg,sizeof(msg),0);
					if (len_msg==0){
						printf("%s cerro conexion\n",inet_ntoa(socketaddr_cli.sin_addr));
						close(i);
						FD_CLR(i, &master);
					}
					else {
						msg[len_msg-1] = '\0';
						printf("%s dice: %s\n",inet_ntoa(socketaddr_cli.sin_addr),msg);

						printf("Tu: ");
						fgets(msg,MAX_MSG+1,stdin);
						send(i,msg,strlen(msg),0);
					}
				}
			}
		}
	}

	return EXIT_SUCCESS;
}
