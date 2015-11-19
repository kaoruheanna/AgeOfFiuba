/*
 * MensajeroRed.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: nico
 */

#include "PairIntSerializable.h"
#include "../GlobalConstants.h"
#include "../Models/MobileModel.h"
#include "../Models/Resource.h"
#include "MensajeroRed.h"
#include "Mensaje.h"
#include "Archivo.h"
#include <stdio.h>
#include "../Utils/Log.h"

static const string TAG = "MensajeroRed";

MensajeroRed::MensajeroRed(int socket) {
	this->socket = socket;
	this->escucha = NULL;
	this->sender = "server";
	this->connectionAlive = false;
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
	Entity* entity = NULL;
	User* user = NULL;
	PairIntSerializable* pair;
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
			case ERROR_MAXIMOS_EQUIPOS:
				resultado = -1;
				break;
			case ESCENARIO:
				configuracion = new Archivo(CONFIG_CLIENT.c_str());
				resultado = recibirSerializable(this->socket, configuracion);
				//printf("MensajeroRed - Recibi escenario con resultado: %i\n", resultado);
				delete configuracion;
				this->escucha->configEscenario(CONFIG_CLIENT);
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
				this->escucha->moverEntidad(modelo, string(recibido->getSender()));
				delete modelo;
				break;
			case INTERACTUAR:
				pair = new PairIntSerializable();
				resultado = recibirSerializable(this->socket, pair);
				//printf("MensajeroRed - Recibi personaje con resultado: %i y sender: %s\n", resultado, recibido->getSender());
				this->escucha->interactuar(pair->first,pair->second);
				delete pair;
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
			case ACTUALIZA_ENTIDAD:
//				Log().Get(TAG) << "Creo entity desde MensajerRed-ActualizaEntidad";
				entity = new Entity();
				resultado = recibirSerializable(this->socket, entity);
//				Log().Get(TAG) << "ACTUALIZA_ENTIDAD: "<<entity->getNombre();
//				Log().Get(TAG) << "Creo entity desde MensajerRed-ActualizaEntidad 2";
				this->escucha->actualizarEntidad(entity);
				delete entity;
				break;
			case CAMBIO_USUARIO:
				user = new User();
				resultado = recibirSerializable(this->socket, user);
				this->escucha->cambioUsuario(user);
				delete user;
				break;
			case CONSTRUIR:
				Log().Get(TAG) << "Creo entity desde MensajerRed-ActualizaEntidad";
				entity = new Entity();
				resultado = recibirSerializable(this->socket, entity);
				this->escucha->construir(entity);
				delete entity;
				break;
			case COMENZO_PARTIDA:
				printf("Cliente - comenzo partida con resultado: %i\n", resultado);
				this->escucha->comenzoPartida();
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
			// TODO diferenciar el tipo real del error (para mostrar mensaje acorde)
			this->escucha->errorDeLogueo();
		}
	}
	this->escucha->errorDeLogueo();
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
void MensajeroRed::configEscenario(const string path) {
	Mensaje* mensaje = new Mensaje(ESCENARIO, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - configEscenario con resultado: %i\n", resultado);
	delete mensaje;
	Archivo* archivo = new Archivo(path.c_str());
	resultado = enviarSerializable(this->socket, archivo);
	printf("Cliente - yaml con resultado: %i\n", resultado);
	delete archivo;
}

void MensajeroRed::actualizarEntidad(Entity* entity) {
	Mensaje* mensaje = new Mensaje(ACTUALIZA_ENTIDAD, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	delete mensaje;
	resultado = enviarSerializable(this->socket, entity);
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

void MensajeroRed::moverEntidad(MobileModel* entity, string username) {
	Mensaje* mensaje = new Mensaje(MOVER_PERSONAJE, username.c_str());
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("Cliente - moverProtagonista con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, entity);
	//printf("Cliente - personaje con resultado: %i\n", resultado);
}


void MensajeroRed::interactuar(int selectedEntityId, int targetEntityId) {
	Mensaje* mensaje = new Mensaje(INTERACTUAR, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Cliente - interactuar entidad %d,%d con resultado: %i\n",selectedEntityId,targetEntityId, resultado);
	delete mensaje;

	resultado = enviarSerializable(this->socket, new PairIntSerializable(selectedEntityId,targetEntityId));
	printf("Cliente - interactuar enviado con resultado: %i\n", resultado);
}

void MensajeroRed::cambioUsuario(User* user) {
	Mensaje* mensaje = new Mensaje(CAMBIO_USUARIO, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	//printf("Cliente - moverProtagonista con resultado: %i\n", resultado);
	delete mensaje;
	resultado = enviarSerializable(this->socket, user);
	//printf("Cliente - personaje con resultado: %i\n", resultado);

}

void MensajeroRed::construir(Entity* entity) {
	Mensaje* mensaje = new Mensaje(CONSTRUIR, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	delete mensaje;
	resultado = enviarSerializable(this->socket, entity);
}

void MensajeroRed::comenzoPartida() {
	Mensaje* mensaje = new Mensaje(COMENZO_PARTIDA, this->sender);
	int resultado = enviarSerializable(this->socket, mensaje);
	printf("Server - comenzo partida con resultado: %i\n", resultado);
	delete mensaje;
}
