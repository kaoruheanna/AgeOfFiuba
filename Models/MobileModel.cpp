/*
 * MobileModel.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "MobileModel.h"
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include "../Utils/Log.h"
#include "../GlobalConstants.h"
#include "../Red/Serializable.h"
using namespace std;

const std::string TAG = "MobileModel";

MobileModel::MobileModel() : Entity("", {0, 0}, 1, 1){
	this->posicion.x = 0;
	this->posicion.y = 0;
	this->moving = false;
	this->Cosechable = false;
}

MobileModel::MobileModel(string nombre, SDL_Point posicion, int ancho_base, int alto_base)
: Entity(nombre, posicion, ancho_base, alto_base){
	this->destinationX = posicion.x;
	this->destinationY = posicion.y;
	this->moving = false;
	this->Cosechable = false;
}

MobileModel::~MobileModel() {
	// TODO Auto-generated destructor stub
}

int MobileModel::getX() {
	return this->posicion.x;
}

int MobileModel::getY() {
	return this->posicion.y;
}

void MobileModel::setX(int x) {
	this->posicion.x = x;
	this->setDestination(this->posicion.x,this->posicion.y);
}

void MobileModel::setY(int y) {
	this->posicion.y = y;
	this->setDestination(this->posicion.x,this->posicion.y);
}

void MobileModel::setDestination(int destinationX, int destionationY) {
	this->destinationX = destinationX;
	this->destinationY = destionationY;
}

int MobileModel::getDestinationX() {
	return this->destinationX;
}

int MobileModel::getDestinationY() {
	return this->destinationY;
}

int MobileModel::getSpeed() {
	return PROTAGONISTA_SPEED;
}

bool MobileModel::updatePosition() {
	bool wasMoving = this->moving;
	double speed = (double)this->getSpeed();

	double deltaX = (double)(this->destinationX - this->posicion.x);
	double deltaY = (double)(this->destinationY - this->posicion.y);

	// si ya estoy en el destino, me fijo si tengo que seguir caminando.
	if ((deltaX == 0) && (deltaY == 0)){
		if (this->camino.empty()){
			this->moving = false;
			return wasMoving!=this->moving;
		}else{//si la cola de camino no esta vacia tengo que seguir caminando
			SDL_Point destination = (SDL_Point)this->camino.front(); //ver casteo
			this->camino.pop();
			this->destinationX = destination.x;
			this->destinationY = destination.y;
			return wasMoving!=this->moving; //aca que deberia devolver?
		}
	}

	this->moving = true;
	double hypotenuse = sqrt (pow(deltaX,2) + pow(deltaY,2.0));
	// si estoy mas cerca que la velocidad, llegue al destino
	if (hypotenuse < speed){
		this->posicion.x = this->destinationX;
		this->posicion.y = this->destinationY;
		return true;
	}

	double displacementX = round(speed * (deltaX / hypotenuse));
	double displacementY = round(speed * (deltaY / hypotenuse));

	this->posicion.x += (int)displacementX;
	this->posicion.y += (int)displacementY;
	return true;
}

bool MobileModel::isMoving() {
	return this->moving;
}

void MobileModel::addResourceToCollect(string resourceName) {
	this->resourcesCounter[resourceName] = 0;
}

void MobileModel::didCollectResource(string resourceName) {
	int value = this->resourcesCounter[resourceName];
	value++;
	this->resourcesCounter[resourceName] = value;
	Log().Get(TAG, logDEBUG) << "resource "<< resourceName <<": "<<this->resourcesCounter[resourceName];
}

list<string> MobileModel::getResourcesNames() {
	list<string> names;
	for(std::map<string,int>::iterator iter = this->resourcesCounter.begin(); iter != this->resourcesCounter.end(); ++iter) 	{
		string name = iter->first;
		names.push_back(name);
	}
	return names;
}

int MobileModel::getValueForResource(string resourceName) {
	return this->resourcesCounter[resourceName];
}

void MobileModel::update(MobileModel* other) {
	this->moving = other->isMoving();
	this->posicion = other->getPosicion();
	for(auto resourceName : this->getResourcesNames()) {
		this->resourcesCounter[resourceName] = other->getValueForResource(resourceName);
	}
}

// TODO mandar y recibir los recursos

// Metodos de serializacion
int MobileModel::getTotalBlockCount() {
	return 5;
}

int MobileModel::getBlockSizeFromIndex(int currentIndex) {
	if(currentIndex == 0){
		return this->serializeStringSize((char*)this->nombre.c_str());
	} else if(currentIndex == 1){
		return sizeof(SDL_Point);
	} else if (currentIndex == 4) {
		return sizeof(bool);
	}

	return sizeof(int);
}

void MobileModel::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex == 0){
		this->serializeString((char*)this->nombre.c_str(), buffer);
	} else if(currentIndex == 1){
		memcpy(buffer, &this->posicion, sizeof(SDL_Point));
	} else if(currentIndex == 2){
		memcpy(buffer, &this->destinationX, sizeof(int));
	} else if (currentIndex == 3){
		memcpy(buffer, &this->destinationY, sizeof(int));
	} else {
		memcpy(buffer, &this->moving, sizeof(int));
	}
}

void MobileModel::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0){
		char* nombre = this->deserializeString(blockData);
		this->nombre = string(nombre);
		free(nombre);
	} else if(currentBlock == 1){
		memcpy(&this->posicion, blockData, sizeof(SDL_Point));
	} else if(currentBlock == 2){
		memcpy(&this->destinationX, blockData, sizeof(int));
	} else if(currentBlock == 3) {
		memcpy(&this->destinationY, blockData, sizeof(int));
	} else {
		memcpy(&this->moving, blockData, sizeof(int));
	}
}

char* MobileModel::deserializeString(void* blockData) {
	char* toDeserialize = (char*) blockData;
	char* string = (char*) malloc(this->serializeStringSize(toDeserialize));
	this->serializeString(toDeserialize, (void*) string);
	return string;
}

void MobileModel::serializeString(char* string, void* buffer) {
	strcpy((char*) buffer, string);
}

int MobileModel::serializeStringSize(char* string) {
	return strlen(string) + 1;
}

