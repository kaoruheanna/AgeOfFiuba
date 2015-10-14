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
#include "SerializableTest/DoubleStringSerializable.h"

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

		// Ejemplo de recibir un string
		/*StringSerializable* serializable = new StringSerializable();
		printf("Servidor - Esperando string\n");
		int resultado = recibirSerializable(client_sd, serializable);
		printf("Servidor - Recibi string con resultado: %i\n", resultado);
		if(resultado > 0){
			printf("Servidor - String recibido: %s\n", serializable->string);
		}
		delete serializable;*/

		// Ejemplo de recibir dos strings
		DoubleStringSerializable* serializable = new DoubleStringSerializable();
		printf("Servidor - Esperando string\n");
		int resultado = recibirSerializable(client_sd, serializable);
		printf("Servidor - Recibi string con resultado: %i\n", resultado);
		if(resultado > 0){
			printf("Servidor - String recibido - 1: %s 2: %s\n", serializable->firstString, serializable->secondString);
		}
		delete serializable;
		// TODO read / write

	//}

	printf("Servidor - Todo ok\n");
	close(sd);
}
