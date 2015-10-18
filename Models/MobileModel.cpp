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
using namespace std;

const std::string TAG = "MobileModel";

MobileModel::MobileModel() : Entity("", {0, 0}, 1, 1){
	this->posicion.x = 0;
	this->posicion.y = 0;
	this->destinationX = 0;
	this->destinationY = 0;
	this->moving = false;
	this->Cosechable = false;
}

MobileModel::MobileModel(string nombre, SDL_Point posicion, int ancho_base, int alto_base)
: Entity(nombre, posicion, ancho_base, alto_base){
	this->destinationX = this->posicion.x;
	this->destinationY = this->posicion.y;
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

int MobileModel::getSpeed() {
	return PROTAGONISTA_SPEED;
}

void MobileModel::updatePosition() {
	double speed = (double)this->getSpeed();

	double deltaX = (double)(this->destinationX - this->posicion.x);
	double deltaY = (double)(this->destinationY - this->posicion.y);

	// si ya estoy en el destino, no hago nada
	if ((deltaX == 0) && (deltaY == 0)){
		this->moving = false;
		return;
	}

	this->moving = true;

	double hypotenuse = sqrt (pow(deltaX,2) + pow(deltaY,2.0));
	// si estoy mas cerca que la velocidad, llegue al destino
	if (hypotenuse < speed){
		this->posicion.x = this->destinationX;
		this->posicion.y = this->destinationY;
		return;
	}

	double displacementX = round(speed * (deltaX / hypotenuse));
	double displacementY = round(speed * (deltaY / hypotenuse));

	this->posicion.x += (int)displacementX;
	this->posicion.y += (int)displacementY;
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
//	Log().Get(TAG, logDEBUG) << "resource "<< resourceName <<": "<<this->resourcesCounter[resourceName];
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


