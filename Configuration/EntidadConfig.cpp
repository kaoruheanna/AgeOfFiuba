/*
 * EntidadConfig.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: nico
 */

#include "EntidadConfig.h"

EntidadConfig::EntidadConfig(int x, int y, std::string tipo) {
	this->x = x;
	this->y = y;
	this->tipo = tipo;
}

EntidadConfig::~EntidadConfig() {
	// TODO Auto-generated destructor stub
}

int EntidadConfig::getX() {
	return this->x;
}
int EntidadConfig::getY() {
	return this->y;
}
std::string EntidadConfig::getTipo() {
	return this->tipo;
}
