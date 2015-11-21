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

struct InfoServidor {
	ServerGameController** modelos;
	string* configFile;
};

Servidor::Servidor() {
	this->modelos = NULL;
}

Servidor::~Servidor() {
}

void* atenderCliente(void* infoCliente);
void* simularModelos(void* modelos);
void* pingearCliente(void* args);

void Servidor::empezar(int port, const char* configPath) {
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
	InfoServidor* serverInfo = (InfoServidor*) malloc(sizeof(InfoServidor));
	serverInfo->configFile = new string(configPath);
	serverInfo->modelos = &this->modelos;
	pthread_create((pthread_t*) malloc(sizeof(pthread_t)), NULL,
			simularModelos, (void*)serverInfo);
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
		char* username = (char*) malloc(strlen(mensaje->getSender()) + 1);
		strcpy(username, mensaje->getSender());
		int errorLogueo = info->servidor->modelos->userLogin(username);
		if(!errorLogueo){
			delete mensaje;
			info->servidor->modelos->setUserActive(username);
			// Si se pudo loguear empieza el flow con el controller
			MensajeroRed* mensajero = new MensajeroRed(info->socket);
			mensajero->setMensajero(info->servidor->modelos);
			info->servidor->modelos->addMensajero(mensajero);
			pthread_t pinger; // Thread que verifica que la conexion sea correcta
			pthread_create(&pinger, NULL, pingearCliente, (void*)mensajero);
			mensajero->esperaMensaje();
			pthread_cancel(pinger);
			info->servidor->modelos->removeMensajero(mensajero);
			info->servidor->modelos->setUserInactive(username);
			delete mensajero;
		} else {
			delete mensaje;
			// TODO diferenciar maximos equipos de nombre tomado
			printf("Servidor - Error de logeo: %i\n", errorLogueo);

			mensaje = new Mensaje(ERROR_NOMBRE_TOMADO, "server");
			resultado = enviarSerializable(info->socket, mensaje);
			printf("Servidor - Responde al mensaje con resultado: %i\n", resultado);
		}
		free(username);
	}

	printf("Servidor - Termino la conexion\n");
	free(arg);
	return NULL;
}

void* pingearCliente(void* args) {
	MensajeroRed* mensajero = (MensajeroRed*) args;
	do{
		mensajero->connectionAlive = false;
		// printf("Server - Checkeando conexion...\n");
		sleep(1);
	} while(mensajero->connectionAlive);
	printf("Server - Cerrando conexion...\n");
	shutdown(mensajero->getSocket(), 2); // No conexion => Cerrar el socket
	return NULL;
}

void* simularModelos(void* arg) {
	InfoServidor* infoServer = (InfoServidor*)arg;
	ServerGameController* modelos = new ServerGameController(new GameConfiguration(*infoServer->configFile));
	(*(infoServer->modelos)) = modelos;
	modelos->init();
	modelos->play();
	delete infoServer->configFile;
	free(infoServer);
	return NULL;
}
