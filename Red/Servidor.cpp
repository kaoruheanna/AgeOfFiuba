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
#include <pthread.h>

#include "../GlobalConstants.h"
#include "Mensaje.h"
#include "MensajeroRed.h"
#include "Archivo.h"

struct InfoCliente {
	Servidor* servidor;
	int socket;
};

Servidor::Servidor() {
	this->modelos = NULL;
}

Servidor::~Servidor() {
}

void* atenderCliente(void* infoCliente);
void* simularModelos(void* modelos);

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
		printf("Servidor - Fallo el listen\n");
		return; // ERR: -1
	}
	pthread_create((pthread_t*) malloc(sizeof(pthread_t)), NULL,
			simularModelos, (void*)&this->modelos);
	while(true){
		printf("Servidor - Esperando un cliente\n");
		sockaddr_in client_addr;
		socklen_t client_length = sizeof(client_addr);
		int client_sd = accept(sd, (sockaddr *) &client_addr, &client_length);
		pthread_t thread;
		InfoCliente* info = (InfoCliente*) malloc(sizeof(InfoCliente));
		info->servidor = this;
		info->socket = client_sd;
		pthread_create(&thread, NULL, atenderCliente, (void*)info);
	}

	close(sd);
}

void* atenderCliente(void* arg) {
	InfoCliente* info = (InfoCliente*) arg;
	printf("Servidor - Empezo la conexion\n");

	// Espera la info del login para el usuario
	Mensaje* mensaje = new Mensaje(VACIO, "server");
	printf("Servidor - Esperando mensaje\n");
	int resultado = recibirSerializable(info->socket, mensaje);
	printf("Servidor - Recibi resultado: %i con mensaje: %s\n", resultado, mensaje->toString());
	if(resultado <= 0){
		delete mensaje;
		printf("Servidor - Se corto la conexion\n");
	} else {
		if(!info->servidor->modelos->userExists(mensaje->getSender()) ||
				!info->servidor->modelos->userActive(mensaje->getSender())){
			if(!info->servidor->modelos->userExists(mensaje->getSender())){
				info->servidor->modelos->addUser(mensaje->getSender());
			}
			delete mensaje;
			// Si se pudo loguear empieza el flow con el controller
			MensajeroRed* mensajero = new MensajeroRed(info->socket);
			mensajero->setMensajero(info->servidor->modelos);
			info->servidor->modelos->addMensajero(mensajero);
			mensajero->esperaMensaje();
			info->servidor->modelos->removeMensajero(mensajero);
			delete mensajero;
		} else {
			delete mensaje;
			mensaje = new Mensaje(ERROR_NOMBRE_TOMADO, "server");
			resultado = enviarSerializable(info->socket, mensaje);
			printf("Servidor - Responde al mensaje con resultado: %i\n", resultado);
		}
	}

	printf("Servidor - Termino la conexion\n");
	free(arg);
	return NULL;
}

void* simularModelos(void* arg) {
	ServerGameController* modelos = new ServerGameController(new GameConfiguration(CONFIG_CUSTOM));
	(*(ServerGameController**)arg) = modelos;
	modelos->init();
	modelos->play();
	return NULL;
}
