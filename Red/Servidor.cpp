/*
 * Servidor.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: nico
 */

#include "Servidor.h"

#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

Servidor::Servidor() {
	// TODO Auto-generated constructor stub

}

Servidor::~Servidor() {
	// TODO Auto-generated destructor stub
}

void Servidor::empezar(int port) {
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("Servidor - Fallo el socket con error %i\n", sd);
		return;
	}
	sockaddr_in addr;
	bzero((char *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sd, (sockaddr *)&addr, sizeof(addr)) == -1){
		printf("Servidor - Fallo el bind\n");
		return; // ERR: -1
	}

	if(listen(sd, 5) == -1){ // 5 == cant max de conecciones
		return; // ERR: -1
	}

	//while(true){

		sockaddr_in client_addr;
		socklen_t client_length = sizeof(client_addr);
		int client_sd = accept(sd, (sockaddr *) &client_addr, &client_length);

		int numer = 0;
		printf("Servidor - Esperando numero: %i\n", numer);
		read(client_sd, &numer, sizeof(int));
		printf("Servidor - Obtenido numero: %i\n", numer);
		// TODO read / write

	//}

	printf("Servidor - Todo ok\n");
	close(sd);
}
