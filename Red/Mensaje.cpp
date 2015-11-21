/*
 * Mensaje.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: nico
 */

#include "Mensaje.h"

#include <string.h>
#include <stdlib.h>

Mensaje::Mensaje() {
	this->sender = NULL;
	this->type = VACIO;
	this->logString = NULL;
}

Mensaje::~Mensaje() {
	if(this->sender != NULL){
		free(this->sender);
		this->sender = NULL;
	}
	if(this->logString != NULL){
		free(this->logString);
		this->logString = NULL;
	}
	this->type = VACIO;
}

Mensaje::Mensaje(MensajeType type, const char* sender) {
	this->type = type;
	this->sender = this->deserializeString((void*) sender);
	this->logString = NULL;
}

// Mensaje methods
MensajeType Mensaje::getType() {
	return this->type;
}
char* Mensaje::getSender() {
	return this->sender;
}
// Serializable methods
int Mensaje::getTotalBlockCount() {
	return 2;
}
int Mensaje::getBlockSizeFromIndex(int currentIndex) {
	if(currentIndex == 0){
		return sizeof(MensajeType);
	} else {
		return this->serializeStringSize(this->sender);
	}
}
void Mensaje::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex == 0){
		memcpy(buffer, &this->type, sizeof(MensajeType));
	} else {
		return this->serializeString(this->sender, buffer);
	}
}
void Mensaje::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0){
		memcpy(&this->type, blockData, sizeof(MensajeType));
	} else {
		if(this->sender != NULL){
			free(this->sender);
			this->sender = NULL;
		}
		this->sender = this->deserializeString(blockData);
	}
}

// string serialization / deserialization
char* Mensaje::deserializeString(void* blockData) {
	char* toDeserialize = (char*) blockData;
	char* string = (char*) malloc(this->serializeStringSize(toDeserialize));
	this->serializeString(toDeserialize, (void*) string);
	return string;
}

void Mensaje::serializeString(char* string, void* buffer) {
	strcpy((char*) buffer, string);
}

int Mensaje::serializeStringSize(char* string) {
	return strlen(string) + 1;
}

// Para logueo
char* Mensaje::toString() {
	char* type = "NULL";
	switch(this->type){
		case VACIO:
			type = "VACIO";
			break;
		case LOGIN:
			type = "LOGIN";
			break;
		case ERROR_NOMBRE_TOMADO:
			type = "ERROR_NOMBRE_TOMADO";
			break;
		case ESCENARIO:
			type = "ESCENARIO";
			break;
		case MOVER_PERSONAJE:
			type = "MOVER_PERSONAJE";
			break;
		case APARECE_PERSONAJE:
			type = "APARECE_PERSONAJE";
			break;
		case PING:
			type = "PING";
			break;
		case ERROR_MAXIMOS_EQUIPOS:
			type = "ERROR_MAXIMOS_EQUIPOS";
			break;
		case CAMBIO_USUARIO:
			type = "CAMBIO_USUARIO";
			break;
		case CONSTRUIR:
			type = "CONSTRUIR";
			break;
		case COMENZO_PARTIDA:
			type = "COMENZO_PARTIDA";
			break;
	}
	char* name = "NULL";
	if(this->sender != NULL){
		name = this->sender;
	}
	if(this->logString != NULL){
		free(this->logString);
		this->logString = NULL;
	}
	this->logString = (char*) malloc(strlen(type) + strlen(name) + 2);
	char* aux = this->logString;
	strcpy(aux, type);
	aux = aux + strlen(type);
	(*aux) = '/';
	aux ++;
	strcpy(aux, name);
	return this->logString;
}
