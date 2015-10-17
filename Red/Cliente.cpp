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

#include "Mensaje.h"
#include "Archivo.h"

using namespace std;


Cliente::Cliente() {
	// TODO Auto-generated constructor stub

}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

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
	// Se intenta logear al server
	Mensaje* mensaje = new Mensaje(LOGIN, userName.c_str());
	int resultado = enviarSerializable(sd, mensaje);
	printf("Cliente - Enviado con estado: %i\nCliente - Esperando respuesta del servidor\n", resultado);
	resultado = recibirSerializable(sd, mensaje);
	printf("Cliente - Recibido estado: %i. Mensaje: %s\n", resultado, mensaje->toString());
	while(!endLoop){
		endLoop = true;
		if(resultado < 0){
			printf("Cliente - Se corto la conexion\n");
		} else if(mensaje->getType() == ERROR_NOMBRE_TOMADO){
			printf("Cliente - El usuario ya existe y esta logeado actualmente\n");
		} else if(mensaje->getType() != ESCENARIO){
			printf("Cliente - Se mando un mensaje inesperado\n");
		} else {
			printf("Cliente - Recibiendo escenario...\n");
			Archivo* configuracion = new Archivo("yaml-files/configuracion_cliente.yaml");
			resultado = recibirSerializable(sd, configuracion);
			delete configuracion;
			printf("Cliente - Recibido con resultado: %i\n", resultado);
			// A partir de aca esta el flow comun de datos...
			// TODO el cliente indica todos los movimientos del jugador
		}
	}
	delete mensaje;
	close(sd);
	printf("Cliente - Termino la conexion\n");
}
