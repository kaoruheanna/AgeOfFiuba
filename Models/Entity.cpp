#include "Entity.h"

#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include "../GlobalConstants.h"
#include "../Utils/Log.h"

using namespace std;

const string TAG ="Entity";

Entity::Entity(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base){
	this->nombre = nombre;
	this->posicion = posicion;
	this->ancho_base = ancho_base;
	this->alto_base = alto_base;
	this->id = id;
}

Entity::Entity(int id,string nombre, int ancho_base, int alto_base){
	this->id = id;
	this->nombre = nombre;
	this->posicion = {-1,-1}; // posicion invalida hace referencia a que no tiene posicion en el mapa.
	this->ancho_base = ancho_base;
	this->alto_base = alto_base;

}

Entity::~Entity(){

}

string Entity::toString(){
	string n ="Entidad con nombre: ";
	n.append(this->nombre);
	return n;
}

string Entity::getNombre() {
	return this->nombre;
}

int Entity::getId() {
	return this->id;
}

SDL_Point Entity::getPosicion(){
	return this->posicion;
}

int Entity::getAnchoBase(){
	return this->ancho_base;
}

int Entity::getAltoBase(){
	return this->alto_base;
}

void Entity::setPosicion(SDL_Point nueva_posicion){
	this -> posicion = nueva_posicion;
}

bool Entity::esJugador() {
	return false;
}

string Entity::getNombreAMostrar() {
	return this->nombre;
}


//Serializar
// Metodos de serializacion
int Entity::getTotalBlockCount() {
	return 3;
}

int Entity::getBlockSizeFromIndex(int currentIndex) {
	if(currentIndex == 0){
		return this->serializeStringSize((char*)this->nombre.c_str());
	} else if (currentIndex == 1 ){
		return sizeof(SDL_Point);
	} else {
		return sizeof(int);
	}
}

void Entity::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex == 0){
		this->serializeString((char*)this->nombre.c_str(), buffer);
	} else if (currentIndex == 1) {
		memcpy(buffer, &this->posicion, sizeof(SDL_Point));
	} else {
		memcpy(buffer, &this->id, sizeof(int));
	}
}

void Entity::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0){
		char* nombre = this->deserializeString(blockData);
		this->nombre = string(nombre);
		free(nombre);
	} else if(currentBlock == 1){
		memcpy(&this->posicion, blockData, sizeof(SDL_Point));
	} else if (currentBlock == 2) {
		memcpy(&this->id, blockData, sizeof(int));
	}
}

char* Entity::deserializeString(void* blockData) {
	char* toDeserialize = (char*) blockData;
	char* string = (char*) malloc(this->serializeStringSize(toDeserialize));
	this->serializeString(toDeserialize, (void*) string);
	return string;
}

void Entity::serializeString(char* string, void* buffer) {
	strcpy((char*) buffer, string);
}

int Entity::serializeStringSize(char* string) {
	return strlen(string) + 1;
}

EntityType Entity::getClass() {
	return ENTITY;
}

bool Entity::admiteNublado() {
	return this->getClass()!=MOBILE_MODEL;
}
