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

#include "Mensaje.h"
#include "Archivo.h"

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
		printf("Servidor - Fallo el listen\n");
		return; // ERR: -1
	}

	while(true){
		printf("Servidor - Esperando un cliente\n");
		sockaddr_in client_addr;
		socklen_t client_length = sizeof(client_addr);
		int client_sd = accept(sd, (sockaddr *) &client_addr, &client_length);
		printf("Servidor - Empezo la conexion\n");
		bool stopClientTalk = false;

		// Espera la info del login para el usuario
		Mensaje* mensaje = new Mensaje(VACIO, "server");
		printf("Servidor - Esperando mensaje\n");
		int resultado = recibirSerializable(client_sd, mensaje);
		printf("Servidor - Recibi resultado: %i con mensaje: %s\n", resultado, mensaje->toString());
		while(!stopClientTalk){
			if(resultado <= 0){
				printf("Servidor - Se corto la conexion\n");
				stopClientTalk = true;
			} else {
				if(!this->existeUsuario(mensaje->getSender()) ||
						!this->usuarioLogueado(mensaje->getSender())){
					delete mensaje;
					mensaje = new Mensaje(ESCENARIO, "server");
					resultado = enviarSerializable(client_sd, mensaje);
					printf("Servidor - Responde al mensaje con resultado: %i\n", resultado);
					Archivo* configuracion = new Archivo("yaml-files/configuracion.yaml");
					resultado = enviarSerializable(client_sd, configuracion);
					delete configuracion;
					printf("Servidor - Responde al mensaje con resultado: %i\n", resultado);
					printf("Servidor - Esperando mensaje\n");
					resultado = recibirSerializable(client_sd, mensaje);
					printf("Servidor - Recibi resultado: %i con mensaje: %s\n", resultado, mensaje->toString());
					// A partir de aca esta el flow comun de datos...
					// TODO el server indica todos los modelos que se cambiaron
				} else {
					delete mensaje;
					mensaje = new Mensaje(ERROR_NOMBRE_TOMADO, "server");
					resultado = enviarSerializable(client_sd, mensaje);
					printf("Servidor - Responde al mensaje con resultado: %i\n", resultado);
					stopClientTalk = true;
				}
			}
		}
		delete mensaje;
		printf("Servidor - Termino la conexion\n");
	}

	close(sd);
}

bool Servidor::existeUsuario(char* nombre) {
	return true;
}
bool Servidor::usuarioLogueado(char* nombre) {
	return true;
}
