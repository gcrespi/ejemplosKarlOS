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
#define MAX_MSG 100

//Prototypes



int main(void) {

	struct sockaddr_in socketaddr_srv;
	socketaddr_srv.sin_family = AF_INET;
	socketaddr_srv.sin_port = htons(3490);
	socketaddr_srv.sin_addr.s_addr = inet_addr("192.168.0.68");
	memset(&socketaddr_srv.sin_zero,'\0',8);

	int socketfd_srv = socket(AF_INET,SOCK_STREAM,0);
	connect(socketfd_srv,(struct sockaddr*) &socketaddr_srv,sizeof(struct sockaddr));

	char msg[MAX_MSG+1];
	int len_msg;
	len_msg = recv(socketfd_srv,msg,MAX_MSG+1,0);
	msg[len_msg] = '\0';
	if (len_msg!=40) {
		do{
			printf("%s dice: %s\n",inet_ntoa(socketaddr_srv.sin_addr),msg);
			printf("Tu: ");
			fgets(msg,MAX_MSG+1,stdin);
			send(socketfd_srv,msg,strlen(msg),0);

			len_msg = recv(socketfd_srv,msg,MAX_MSG+1,0);
			msg[len_msg-1] = '\0';
		}while(len_msg!=0 && msg[0]!='\0');
		if(len_msg==0) puts("El servidor se ha desconectado");
	} else {
		printf("%s dice: %s\n",inet_ntoa(socketaddr_srv.sin_addr),msg);
	}
	return EXIT_SUCCESS;
}
