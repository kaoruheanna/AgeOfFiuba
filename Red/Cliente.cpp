/*
 * Cliente.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: nico
 */

#include "Cliente.h"

#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../Controllers/Mensajero.h"

#include "Mensaje.h"
#include "Archivo.h"
#include "MensajeroRed.h"
#include "../Controllers/ClientGameController.h"

using namespace std;


Cliente::Cliente() {
	// TODO Auto-generated constructor stub

}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

struct InfoLoguearse {
	MensajeroRed* mensajero;
	ClientGameController* controller;
	const char* nombre;
};

void* loguearse(void* args);

void Cliente::empezar(char* ip, int port) {
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("Cliente - Fallo el socket con error %i\n", sd);
		return;
	}
	sockaddr_in addr;
	bzero((char *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	struct hostent *ptr_srv;
	ptr_srv = gethostbyname(ip);
	if(ptr_srv == NULL){
		printf("Cliente - Fallo el host name\n");
		return; // ERR: -2
	}

	memcpy(&addr.sin_addr,
			ptr_srv->h_addr_list[0],
			ptr_srv->h_length);

	if(connect(sd, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		printf("Cliente - Fallo la conexion\n");
		close(sd);
		return ; // ERR: -1
	}
	printf("Cliente - Empezo la conexion\n");
	// Loop de conexion
	bool endLoop = false;
	string userName;
	do{
		if(endLoop){
			printf("Nombre invalido. Escriba otro: ");
		} else {
			printf("Escribir nombre: ");
		}
		getline(cin, userName);
		endLoop = (userName.length() == 0);
	} while(endLoop);
	// Crea controller y client e intenta conectarse al server por nombre
	MensajeroRed* mensajero = new MensajeroRed(sd);
	ClientGameController *clientGameController = new ClientGameController(mensajero);
	mensajero->setMensajero(clientGameController);
	InfoLoguearse* info = (InfoLoguearse*) malloc(sizeof(InfoLoguearse));
	info->mensajero = mensajero;
	info->controller = clientGameController;
	info->nombre = userName.c_str();
	pthread_t thread;
	pthread_create(&thread, NULL, loguearse, (void*)info);
	mensajero->esperaMensaje();
	pthread_cancel(thread);

	delete clientGameController;
	delete mensajero;

	close(sd);
	printf("Cliente - Termino la conexion\n");
}

void* loguearse(void* args) {
	InfoLoguearse* info = (InfoLoguearse*) args;
	info->mensajero->loguearse((char*)info->nombre);
	info->controller->play();
	free(args);
	return NULL;
}
