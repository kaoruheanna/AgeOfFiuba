/*
 * MensajeroRed.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: nico
 */

#include "../GlobalConstants.h"
#include "MensajeroRed.h"
#include "Mensaje.h"
#include "Archivo.h"
#include <stdio.h>

MensajeroRed::MensajeroRed(int socket) {
	// TODO Auto-generated constructor stub
	this->socket = socket;
	this->escucha = NULL;
}

MensajeroRed::~MensajeroRed() {
	// TODO Auto-generated destructor stub
}


void MensajeroRed::setMensajero(Mensajero* escucha) {
	this->escucha = escucha;
}

void MensajeroRed::esperaMensaje() {
	Mensaje* recibido = new Mensaje(VACIO, "nada");
	Archivo* configuracion = NULL;
	int resultado = recibirSerializable(this->socket, recibido);
	while(resultado > 0){
		switch(recibido->getType()){
			case LOGIN:
				this->escucha->loguearse(recibido->getSender());
				break;
			case ERROR_NOMBRE_TOMADO:
				this->escucha->errorDeLogueo();
				resultado = -1; // Usuario ya tomado
				break;
			case ESCENARIO:
				configuracion = new Archivo(CONFIG_CLIENT.c_str());
				resultado = recibirSerializable(this->socket, configuracion);
				delete configuracion;
				this->escucha->configEscenario(CONFIG_CLIENT);
				break;
			default:
				resultado = -1; // No se pudo entender el mensaje => Cerrar la conexion
		}
		if(resultado > 0){
			resultado = recibirSerializable(this->socket, recibido);
		}
	}
	delete recibido;
}

// Metodos Servidor -> Cliente
void MensajeroRed::errorDeLogueo() {
	Mensaje* mensaje = new Mensaje(ERROR_NOMBRE_TOMADO, "server");
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Servidor - Responde al mensaje con resultado: %i\n", resultado);
	delete mensaje;
}
void MensajeroRed::configEscenario(const string path) {
	Mensaje* mensaje = new Mensaje(ESCENARIO, "server");
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - configEscenario con resultado: %i\n", resultado);
	delete mensaje;
	Archivo* archivo = new Archivo(path.c_str());
	resultado = enviarSerializable(this->socket, archivo);
	printf("Cliente - yaml con resultado: %i\n", resultado);
	delete archivo;
}
//virtual void apareceRecurso(Resource* recurso);
//virtual void desapareceRecurso(Resource* recurso);
//virtual void actualizaPersonaje(MobileModel* entity);
// Metodos Cliente -> Servidor
void MensajeroRed::loguearse(char* nombre) {
	Mensaje* mensaje = new Mensaje(LOGIN, nombre);
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - loguearse con resultado: %i\n", resultado);
	delete mensaje;
}
