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
#include <iostream>

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

		int stringLength = 0;
		printf("Servidor - Esperando numero\n");
		read(client_sd, &stringLength, sizeof(int));
		printf("Servidor - Obtenido numero: %i\n", stringLength);
		printf("Servidor - Esperando string\n");
		char* puntero = (char*) malloc(stringLength);
		char* posicionActual = puntero;
		int leido = read(client_sd, posicionActual, stringLength);
		// Mientras no sea un error (leido < 0) y no haya terminado la lectura seguir esperando
		while((leido > 0) && ((stringLength - leido) > 0)){
			printf("Servidor - Leido: %i\n", leido);
			stringLength -= leido;
			posicionActual = posicionActual + leido;
			printf("Servidor - Esperando resto de string: %i\n", stringLength);
			leido = read(client_sd, posicionActual, stringLength);
		}
		if(leido > 0){
			printf("Servidor - Leido string: %s\n", puntero);
		} else {
			printf("Servidor - No se pudo leer nada: %i\n", leido);
		}
		// TODO read / write

	//}

	printf("Servidor - Todo ok\n");
	close(sd);
}
