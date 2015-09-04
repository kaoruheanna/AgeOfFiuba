/*
 * MobileModel.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "MobileModel.h"

MobileModel::MobileModel() {
	this->x = 0;
	this->y = 0;
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
}

void MobileModel::setY(int y) {
	this->y = y;
}


