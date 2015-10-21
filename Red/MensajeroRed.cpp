/*
 * MensajeroRed.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: nico
 */

#include "../GlobalConstants.h"
#include "../Models/MobileModel.h"
#include "../Models/Resource.h"
#include "MensajeroRed.h"
#include "Mensaje.h"
#include "Archivo.h"
#include <stdio.h>

MensajeroRed::MensajeroRed(int socket) {
	this->socket = socket;
	this->escucha = NULL;
	this->sender = "server";
}

MensajeroRed::~MensajeroRed() {
}


void MensajeroRed::setMensajero(Mensajero* escucha) {
	this->escucha = escucha;
}

void MensajeroRed::esperaMensaje() {
	Mensaje* recibido = new Mensaje(VACIO, "nada");
	Archivo* configuracion = NULL;
	MobileModel* modelo = NULL;
	Resource* resource = NULL;
	FogOfWar* fog = NULL;
	int resultado = recibirSerializable(this->socket, recibido);
	printf("MensajeroRed - Recibi resultado: %i con mensaje: %s\n", resultado, recibido->toString());
	while(resultado > 0){
		switch(recibido->getType()){
			case LOGIN:
				this->escucha->loguearse(recibido->getSender());
				break;
			case ERROR_NOMBRE_TOMADO:
				resultado = -1;
				break;
			case ESCENARIO:
				configuracion = new Archivo(CONFIG_CLIENT.c_str());
				resultado = recibirSerializable(this->socket, configuracion);
				//printf("MensajeroRed - Recibi escenario con resultado: %i\n", resultado);
				delete configuracion;
				fog = new FogOfWar();
				resultado = recibirSerializable(this->socket, fog);
				this->escucha->configEscenario(CONFIG_CLIENT, fog);
				break;
			case APARECE_PERSONAJE:
				modelo = new MobileModel();
				resultado = recibirSerializable(this->socket, modelo);
				//printf("MensajeroRed - Recibi personaje con resultado: %i\n", resultado);
				this->escucha->actualizaPersonaje(modelo);
				delete modelo;
				break;
			case MOVER_PERSONAJE:
				modelo = new MobileModel();
				resultado = recibirSerializable(this->socket, modelo);
				//printf("MensajeroRed - Recibi personaje con resultado: %i y sender: %s\n", resultado, recibido->getSender());
				this->escucha->moverProtagonista(modelo);
				delete modelo;
				break;
			case APARECE_RECURSO:
				resource = new Resource();
				resultado = recibirSerializable(this->socket, resource);
				//printf("MensajeroRed - Recibi aparece resource con resultado: %i\n", resultado);
				this->escucha->apareceRecurso(resource);
				delete resource;
				break;
			case DESAPARECE_RECURSO:
				resource = new Resource();
				resultado = recibirSerializable(this->socket, resource);
				//printf("MensajeroRed - Recibi  desaparece resource con resultado: %i\n", resultado);
				this->escucha->desapareceRecurso(resource);
				delete resource;
				break;
			case PING:
				break;
			default: // No se pudo entender el mensaje
				resultado = -1;
		}
		this->connectionAlive = (resultado > 0);
		if(resultado > 0){
			resultado = recibirSerializable(this->socket, recibido);
		} else {
			this->escucha->errorDeLogueo();
		}
	}
	delete recibido;
}

int MensajeroRed::getSocket() {
	return this->socket;
}

void MensajeroRed::ping(){
	Mensaje* mensaje = new Mensaje(PING, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("MensajeroRed - Pingea con resultado: %i\n", resultado);
	delete mensaje;
}

// Metodos Servidor -> Cliente
void MensajeroRed::errorDeLogueo() {
	Mensaje* mensaje = new Mensaje(ERROR_NOMBRE_TOMADO, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("Servidor - Responde al mensaje con resultado: %i\n", resultado);
	delete mensaje;
}
void MensajeroRed::configEscenario(const string path, FogOfWar* fog) {
	Mensaje* mensaje = new Mensaje(ESCENARIO, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - configEscenario con resultado: %i\n", resultado);
	delete mensaje;
	Archivo* archivo = new Archivo(path.c_str());
	resultado = enviarSerializable(this->socket, archivo);
	printf("Cliente - yaml con resultado: %i\n", resultado);
	delete archivo;
	resultado = enviarSerializable(this->socket, fog);
}

void MensajeroRed::apareceRecurso(Resource* recurso) {
	Mensaje* mensaje = new Mensaje(APARECE_RECURSO, this->sender);
	//printf("Cliente - apareceRecurso para enviar\n");
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("Cliente - apareceRecurso con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, recurso);
	//printf("Cliente - apareceRecurso con resultado: %i\n", resultado);
}

void MensajeroRed::desapareceRecurso(Resource* recurso) {
	Mensaje* mensaje = new Mensaje(DESAPARECE_RECURSO, this->sender);
	//printf("Cliente - desapareceRecurso para enviar\n");
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("Cliente - desapareceRecurso con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, recurso);
	//printf("Cliente - desapareceRecurso con resultado: %i\n", resultado);
}

void MensajeroRed::actualizaPersonaje(MobileModel* entity) {
	Mensaje* mensaje = new Mensaje(APARECE_PERSONAJE, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("Cliente - actualizaPersonaje con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, entity);
	//printf("Cliente - personaje con resultado: %i\n", resultado);
}
// Metodos Cliente -> Servidor
void MensajeroRed::loguearse(char* nombre) {
	this->sender = nombre;
	Mensaje* mensaje = new Mensaje(LOGIN, nombre);
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - loguearse con resultado: %i\n", resultado);
	delete mensaje;
}

void MensajeroRed::moverProtagonista(MobileModel* entity) {
	Mensaje* mensaje = new Mensaje(MOVER_PERSONAJE, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("Cliente - moverProtagonista con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, entity);
	//printf("Cliente - personaje con resultado: %i\n", resultado);
}
