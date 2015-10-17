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
	// TODO Auto-generated constructor stub
	this->socket = socket;
	this->escucha = NULL;
	this->busy = false;
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
	MobileModel* modelo = NULL;
	Resource* resource = NULL;
	int resultado = recibirSerializable(this->socket, recibido);
	printf("MensajeroRed - Recibi resultado: %i con mensaje: %s\n", resultado, recibido->toString());
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
				printf("MensajeroRed - Recibi escenario con resultado: %i\n", resultado);
				delete configuracion;
				this->escucha->configEscenario(CONFIG_CLIENT);
				break;
			case APARECE_PERSONAJE:
				while(this->busy){
					SDL_Delay(DELAY_MILISEC);
				}
				this->busy = true;
				modelo = new MobileModel();
				resultado = recibirSerializable(this->socket, modelo);
				printf("MensajeroRed - Recibi personaje con resultado: %i\n", resultado);
				this->escucha->actualizaPersonaje(modelo);
				delete modelo;
				this->busy = false;
				break;
			case MOVER_PERSONAJE:
				while(this->busy){
					SDL_Delay(DELAY_MILISEC);
				}
				this->busy = true;
				modelo = new MobileModel();
				resultado = recibirSerializable(this->socket, modelo);
				printf("MensajeroRed - Recibi personaje con resultado: %i\n", resultado);
				this->escucha->moverProtagonista(modelo);
				delete modelo;
				this->busy = false;
				break;

			case APARECE_RECURSO:
				while(this->busy){
					SDL_Delay(DELAY_MILISEC);
				}
				this->busy = true;
				resource = new Resource();
				resultado = recibirSerializable(this->socket, resource);
				printf("MensajeroRed - Recibi resource con resultado: %i\n", resultado);
				this->escucha->apareceRecurso(resource);
				delete resource;
				this->busy = false;
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
void MensajeroRed::apareceRecurso(Resource* recurso) {
	while(this->busy){
		SDL_Delay(DELAY_MILISEC);
	}
	this->busy = true;
	Mensaje* mensaje = new Mensaje(APARECE_RECURSO, "server");
	printf("Cliente - apareceRecurso para enviar");
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - apareceRecurso con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, recurso);
	printf("Cliente - apareceRecurso con resultado: %i\n", resultado);
	this->busy = false;
}

//virtual void desapareceRecurso(Resource* recurso);
void MensajeroRed::actualizaPersonaje(MobileModel* entity) {
	while(this->busy){
		SDL_Delay(DELAY_MILISEC);
	}
	this->busy = true;
	Mensaje* mensaje = new Mensaje(APARECE_PERSONAJE, "server");
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - actualizaPersonaje con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, entity);
	printf("Cliente - personaje con resultado: %i\n", resultado);
	this->busy = false;
}
// Metodos Cliente -> Servidor
void MensajeroRed::loguearse(char* nombre) {
	Mensaje* mensaje = new Mensaje(LOGIN, nombre);
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - loguearse con resultado: %i\n", resultado);
	delete mensaje;
}

void MensajeroRed::moverProtagonista(MobileModel* entity) {
	while(this->busy){
		SDL_Delay(DELAY_MILISEC);
	}
	this->busy = true;
	Mensaje* mensaje = new Mensaje(MOVER_PERSONAJE, "todo"); // TODO agregar el nombre
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - moverProtagonista con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, entity);
	printf("Cliente - personaje con resultado: %i\n", resultado);
	this->busy = false;
}
