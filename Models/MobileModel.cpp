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
#include <cmath>
#include "../Utils/Log.h"
#include "../GlobalConstants.h"
#include "../Red/Serializable.h"
using namespace std;

const std::string TAG = "MobileModel";

MobileModel::MobileModel() : Entity(0,"", {0, 0}, 1, 1){
	this->posicion.x = 0;
	this->posicion.y = 0;
	this->destinationX = 0;
	this->destinationY = 0;
	this->moving = false;
	this->username = "";
	this->userActive = false;
}

MobileModel::MobileModel(int id, string nombre, SDL_Point posicion, int ancho_base, int alto_base)
: Entity(id,nombre, posicion, ancho_base, alto_base){
	this->destinationX = posicion.x;
	this->destinationY = posicion.y;
	this->moving = false;
	this->username = "";
	this->userActive = false;
}

MobileModel::~MobileModel() {
	// TODO Auto-generated destructor stub
}

bool MobileModel::isActive() {
	return this->userActive;
}

void MobileModel::setActive(bool active) {
	this->userActive = active;
	if(!active){ // Hacer que se deje de mover al desactivarse
		this->olvidarCamino();
	}
}

void MobileModel::olvidarCamino(){
	this->clearPath();
	this->moving = false;
	this->setDestination(this->getX(), this->getY());
}

string MobileModel::getUsername() {
	return this->username;
}

void MobileModel::setUsername(string username) {
	this->username = string(username);
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
	bool wasMoving = this -> moving;
	double speed = (double)this->getSpeed();

	double deltaX = (double)(this->destinationX - this->posicion.x);
	double deltaY = (double)(this->destinationY - this->posicion.y);

	// si ya estoy en el destino, me fijo si tengo que seguir caminando.
	if ((deltaX == 0) && (deltaY == 0)){
		if (this->path.empty()){
			this->moving = false;
			return wasMoving != this->moving;
		}else{//si la cola de camino no esta vacia tengo que seguir caminando
			SDL_Point destination = this->getNextDestination();
			this->destinationX = destination.x;
			this->destinationY = destination.y;
			deltaX = (double)(this->destinationX - this->posicion.x);
			deltaY = (double)(this->destinationY - this->posicion.y);
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
	this->setTeam(other->getTeam());
	this->userActive = other->isActive();
	this->moving = other->isMoving();
	this->posicion = other->getPosicion();
	for(auto resourceName : this->getResourcesNames()) {
		this->resourcesCounter[resourceName] = other->getValueForResource(resourceName);
	}
}


void MobileModel::addDestination(int destinationX, int destionationY){
	this->path.push({destinationX,destinationY});
}

SDL_Point MobileModel::getNextDestination(){
	SDL_Point destination = this->path.front();
	this->path.pop();
	return destination;
}

SDL_Point MobileModel::getNextPosition(){
	if (!this->moving){
		return this->posicion;
	}

	double speed = (double)this->getSpeed();
	double deltaX = (double)(this->destinationX - this->posicion.x);
	double deltaY = (double)(this->destinationY - this->posicion.y);
	double hypotenuse = sqrt (pow(deltaX,2) + pow(deltaY,2.0));

	if (hypotenuse < speed){return {this->destinationX, this->destinationY};}

	double displacementX = round(speed * (deltaX / hypotenuse));
	double displacementY = round(speed * (deltaY / hypotenuse));

	int x = this->posicion.x + (int)displacementX;
	int y = this->posicion.y + (int)displacementY;
	return {x,y};
}

void MobileModel::setPath(queue<SDL_Point> new_path){
	this->path.swap(new_path);
}

void MobileModel::clearPath(){
	queue<SDL_Point,deque<SDL_Point>> empty;
	this->path.swap(empty);
}

bool MobileModel::esJugador() {
	return true;
}

string MobileModel::getNombreAMostrar(){
	return this->username;
}

EntityType MobileModel::getClass() {
	return MOBILE_MODEL;
}

// Metodos de serializacion
//TODO desharcodear los recursos o harcodearlos en todos lados igual
int MobileModel::getTotalBlockCount() {
	return 8 + Entity::getTotalBlockCount();
}

int MobileModel::getBlockSizeFromIndex(int currentIndex) {
	int realIndex =  currentIndex - Entity::getTotalBlockCount();
	if (	(realIndex == 0) ||
			(realIndex == 1) ||
			(realIndex == 4) ||
			(realIndex == 5) ||
			(realIndex == 6)) {
		return sizeof(int);
	} else if (realIndex == 2) {
		return sizeof(bool);
	} else if(realIndex == 3){
		return this->serializeStringSize((char*) this->username.c_str());
	} else if(realIndex == 7){
		return sizeof(bool);
	}
	return Entity::getBlockSizeFromIndex(currentIndex);
}

void MobileModel::getBlockFromIndex(int currentIndex, void* buffer) {
	int realIndex =  currentIndex - Entity::getTotalBlockCount();
	if (realIndex < 0) {
		Entity::getBlockFromIndex(currentIndex, buffer);
	} else if(realIndex == 0){
		memcpy(buffer, &this->destinationX, sizeof(int));
	} else if (realIndex == 1){
		memcpy(buffer, &this->destinationY, sizeof(int));
	} else if (realIndex == 2) {
		memcpy(buffer, &this->moving, sizeof(bool));
	} else if (realIndex == 3) {
		this->serializeString((char*) this->username.c_str(), buffer);
	} else if(realIndex == 4){
		memcpy(buffer, &this->resourcesCounter["comida"], sizeof(int));
	} else if (realIndex == 5){
		memcpy(buffer, &this->resourcesCounter["madera"], sizeof(int));
	} else if(realIndex == 6){
		memcpy(buffer, &this->resourcesCounter["piedra"], sizeof(int));
	} else if(realIndex == 7){
		memcpy(buffer, &this->userActive, sizeof(bool));
	}
}

void MobileModel::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	int realBlock =  currentBlock - Entity::getTotalBlockCount();
	if (realBlock < 0) {
		Entity::deserialize(totalBlockCount,currentBlock,blockData);
	} else if(realBlock == 0){
		memcpy(&this->destinationX, blockData, sizeof(int));
	} else if(realBlock == 1) {
		memcpy(&this->destinationY, blockData, sizeof(int));
	} else if(realBlock == 2){
		memcpy(&this->moving, blockData, sizeof(bool));
	} else if (realBlock == 3) {
		char* username = this->deserializeString(blockData);
		this->username = string(username);
		free(username);
	}  else if(realBlock == 4){
		memcpy(&this->resourcesCounter["comida"], blockData, sizeof(int));
	} else if(realBlock == 5) {
		memcpy(&this->resourcesCounter["madera"], blockData, sizeof(int));
	} else if(realBlock == 6) {
		memcpy(&this->resourcesCounter["piedra"], blockData, sizeof(int));
	} else if(realBlock == 7){
		memcpy(&this->userActive, blockData, sizeof(bool));
	}
}

