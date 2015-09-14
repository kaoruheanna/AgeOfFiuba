/*
 * PantallaConfig.cpp
 *
 *  Created on: 13/09/2015
 *      Author: die89
 */

#include "PantallaConfig.h"

PantallaConfig::PantallaConfig(YAML::Node nodo) {
	this->nodoPantalla = nodo;
	this->verificarAltoYAncho();
}

PantallaConfig::PantallaConfig() {
	// TODO Auto-generated destructor stub
}

PantallaConfig::~PantallaConfig() {
	// TODO Auto-generated destructor stub
}

void PantallaConfig::verificarAltoYAncho(){
	if (this->nodoPantalla.size() == 2){
		if (this->verificarAlto() && this->verificarAncho()){
			std::cout << "ALTO Y ANCHO CORRECTOS" << std::endl;
			return;
		}
	}
	std::cout << "ALTO Y ANCHO INVALIDOS" << std::endl;
	this->nodoPantalla[0]["alto"] = ALTO_PANTALLA_DEFAULT;
	this->nodoPantalla[1]["ancho"] = ANCHO_PANTALLA_DEFAULT;
	return;
}

//Verifica que los valores no sean negativos, falta verificar que no sean de otro tipo
bool PantallaConfig::verificarAlto(){
	if (this->nodoPantalla[0]["alto"] && this->nodoPantalla[0]["alto"].IsScalar() && this->nodoPantalla[0]["alto"].as<int>() > 0){
		return true;
	}
	return false;
}

bool PantallaConfig::verificarAncho(){
	if (this->nodoPantalla[1]["ancho"] && this->nodoPantalla[1]["ancho"].IsScalar() && this->nodoPantalla[1]["ancho"].as<int>() > 0){
		return true;
	}
	return false;
}


int PantallaConfig::getAlto(){
	return this->nodoPantalla[0]["alto"].as<int>();
}

int PantallaConfig::getAncho(){
	return this->nodoPantalla[1]["ancho"].as<int>();
}

