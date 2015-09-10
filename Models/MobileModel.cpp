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
using namespace std;

MobileModel::MobileModel() {
	this->x = 0;
	this->y = 0;
	this->destinationX = 0;
	this->destinationY = 0;
}

MobileModel::~MobileModel() {
	// TODO Auto-generated destructor stub
}

int MobileModel::getX() {
	return this->x;
}

int MobileModel::getY() {
	return this->y;
}

void MobileModel::setX(int x) {
	this->x = x;
	this->setDestination(this->x,this->y);
}

void MobileModel::setY(int y) {
	this->y = y;
	this->setDestination(this->x,this->y);
}

void MobileModel::setDestination(int destinationX, int destionationY) {
	this->destinationX = destinationX;
	this->destinationY = destionationY;
}

int MobileModel::getSpeed() {
	return 5;
}

void MobileModel::updatePosition() {
	double speed = (double)this->getSpeed();

	double deltaX = (double)(this->destinationX - this->x);
	double deltaY = (double)(this->destinationY - this->y);

	// si ya estoy en el destino, no hago nada
	if ((deltaX == 0) && (deltaY == 0)){
		return;
	}

	double hypotenuse = sqrt (pow(deltaX,2) + pow(deltaY,2.0));

	// si estoy mas cerca que la velocidad, llegue al destino
	if (hypotenuse < speed){
		this->x = this->destinationX;
		this->y = this->destinationY;
		return;
	}

	double displacementX = round(speed * (deltaX / hypotenuse));
	double displacementY = round(speed * (deltaY / hypotenuse));

//	Log().Get(logINFO) << "displacementX: " << displacementX << " displacementY: " << displacementY;

	this->x += (int)displacementX;
	this->y += (int)displacementY;
}




