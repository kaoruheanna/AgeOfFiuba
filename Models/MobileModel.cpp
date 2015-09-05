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

double MobileModel::getAngle() {
	double deltaX = (double)(this->destinationX - this->x);
	double deltaY = (double)(this->destinationY - this->y);
	double angle = 0;

	if (deltaX != 0){
		angle = atan2(deltaY,deltaX);
	} else if (deltaY > 0) {
		angle = M_PI_2;
	} else if (deltaY < 0) {
		angle = (3.0 * M_PI_2);
	}

	Log().Get(logINFO) << "angulo: " << (angle * 180.0 / M_PI);
	return angle;
}

void MobileModel::updatePosition() {
	float speed = (double)this->getSpeed();

	float deltaX = (double)(this->destinationX - this->x);
	float deltaY = (double)(this->destinationY - this->y);

	// si ya estoy en el destino, no hago nada
	if ((deltaX == 0) && (deltaY == 0)){
		return;
	}

	float hypotenuse = sqrt (pow(deltaX,2) + pow(deltaY,2.0));
	hypotenuse = fmin(speed,hypotenuse);

	double angle = this->getAngle();
	double displacementX = round(hypotenuse * cos(angle));
	double displacementY = round(hypotenuse * sin(angle));

	//	cout << "angulo: " << (angle * 180 / 3.14159265) << endl;
//	cout << "angulo: " << angle << " displacementX: " << displacementX << " displacementY: " << displacementY << endl << endl;

	this->x += (int)displacementX;
	this->y += (int)displacementY;
}




