/*
 ============================================================================
 Name        : chat.c
 Author      : Franco
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

#define DEST_IP "127.0.0.1"
#define DEST_PORT 5552
#define MYPORT 5553

void solicitarConexionChat()
{
	int sockfd;
	struct sockaddr_in my_addr;

	sockfd = socket(AF_INET, SOCK_STREAM,0);

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(MYPORT);
//	my_addr.sin_addr.s_addr= inet_addr("");
	my_addr.sin_addr.s_addr=INADDR_ANY;

	memset(&(my_addr.sin_zero),'\0', 8);


	bind(sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr));
}






int main(void) {

	printf("bienvenido al chat\n");

	int sockfd;
	struct sockaddr_in dest_addr;
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(DEST_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
	memset(&(dest_addr.sin_zero),'\0',8);

	connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));

	char msg[50];
	char aux[50];
	int len, bytes_sent;

	while(1){

		fgets(msg,strlen(msg)+1,stdin);
		len= strlen(msg)+1;
		sprintf(aux,"%i",len);
		send(sockfd,aux,4,0);
		send(sockfd,msg,len,0);

		recv(sockfd,msg,4,0);
		len=atoi(msg);

		recv(sockfd,msg,len,0);
		printf("Envio: %s\n",msg);

	}
	close(sockfd);

	return EXIT_SUCCESS;
}
