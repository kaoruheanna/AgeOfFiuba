/*
 * PantallaConfig.cpp
 *
 *  Created on: 13/09/2015
 *      Author: die89
 */

#include "PantallaConfig.h"
#include "../Utils/Log.h"

const std::string TAG = "PantallaConfig";

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
	if (this->nodoPantalla.IsSequence() && this->nodoPantalla.size() == 2){
		if (this->verificarAlto() && this->verificarAncho()){
			return;
		}
	}
//	Log().Get(TAG,logWARNING) << "Alto y ancho de pantalla incorrectos, utilizando valores por defecto";
	this->nodoPantalla[0]["alto"] = ALTO_PANTALLA_DEFAULT;
	this->nodoPantalla[1]["ancho"] = ANCHO_PANTALLA_DEFAULT;
	return;
}

//Verifica que los valores no sean negativos, falta verificar que no sean de otro tipo
bool PantallaConfig::verificarAlto(){
	if (this->nodoPantalla[0]["alto"] && !this->nodoPantalla[0]["alto"].IsNull() && this->nodoPantalla[0]["alto"].IsScalar()){
		try{
			if(nodoPantalla[0]["alto"].as<int>() > 0){
			return true;
			}
		}catch(YAML::RepresentationException& error){
		return false;
		}
	}
	return false;
}

bool PantallaConfig::verificarAncho(){
	if (this->nodoPantalla[1]["ancho"] && !this->nodoPantalla[1]["ancho"].IsNull() && this->nodoPantalla[1]["ancho"].IsScalar()){
		try{
				if(nodoPantalla[1]["ancho"].as<int>() > 0){
					return true;
				}
		} catch(YAML::RepresentationException& error){
		return false;
		}
	}
	return false;
}


int PantallaConfig::getAlto(){
	return this->nodoPantalla[0]["alto"].as<int>();
}

int PantallaConfig::getAncho(){
	return this->nodoPantalla[1]["ancho"].as<int>();
}

