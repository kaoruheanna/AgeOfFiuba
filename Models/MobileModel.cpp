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
	return 10;
}

void MobileModel::updatePosition() {
	float speed = (float)this->getSpeed();

	float deltaX = (float)(this->destinationX - this->x);
	float deltaY = (float)(this->destinationY - this->y);

	// si ya estoy en el destino, no hago nada
	if ((deltaX == 0) && (deltaY == 0)){
		return;
	}

	float hypotenuse = sqrt (pow(deltaX,2) + pow(deltaY,2.0));
//	cout << "deltaX=" << deltaX << ", deltaY=" << deltaY << " hipotenusa=" << hypotenuse << endl;
	hypotenuse = fmin(speed,hypotenuse);
	hypotenuse = 1;
//	cout << "entonces me quedo hypotenusa: " << hypotenuse << endl;

	// si delta X es cero, no puedo calcular el angulo
	if (deltaX == 0){
		this->y += (int)hypotenuse;
		return;
	}

	float angle =atan2(deltaY,deltaX);
	float displacementX = round(hypotenuse * cos(angle));
	float displacementY = round(hypotenuse * sin(angle));
//	cout << "angulo: " << (angle * 180 / 3.14159265) << endl;
//	cout << "angulo: " << angle << " displacementX: " << displacementX << " displacementY: " << displacementY << endl << endl;

	this->x += (int)displacementX;
	this->y += (int)displacementY;
}




