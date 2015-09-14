/*
 * ConfiguracionConfig.cpp
 *
 *  Created on: 13/09/2015
 *      Author: die89
 */

#include "ConfiguracionConfig.h"

ConfiguracionConfig::ConfiguracionConfig() {
	// TODO Auto-generated constructor stub

}

ConfiguracionConfig::ConfiguracionConfig(YAML::Node nodo) {
	this->nodoConfiguracion = nodo;
	this->verificarDatos();
}

ConfiguracionConfig::~ConfiguracionConfig() {
	// TODO Auto-generated destructor stub
}

void ConfiguracionConfig::verificarDatos(){
		if (!this->verificarVelocidad()){
			std::cout << "VELOCIDAD DE PERSONAJE INVALIDA, SE UTILIZA EL VALOR POR DEFECTO" << std::endl;
			this->nodoConfiguracion[0]["vel_personaje"] = VELOCIDAD_PERSONAJE_DEFAULT;
		}
		if (!this->verificarScroll()){
			std::cout << "MARGEN DE SCROLL INVALIDO, SE UTILIZA EL VALOR POR DEFECTO" << std::endl;
			this->nodoConfiguracion[1]["margen_scroll"] = MARGEN_SCROLL_DEFAULT;
		}
}

bool ConfiguracionConfig::verificarVelocidad(){
	if (this->nodoConfiguracion[0]["vel_personaje"] && this->nodoConfiguracion[0]["vel_personaje"].IsScalar() && this->nodoConfiguracion[0]["vel_personaje"].as<int>() > 0){
		return true;
	}
	return false;
}

bool ConfiguracionConfig::verificarScroll(){
	if (this->nodoConfiguracion[1]["margen_scroll"] && this->nodoConfiguracion[1]["margen_scroll"].IsScalar() && this->nodoConfiguracion[1]["margen_scroll"].as<int>() > 0){
		return true;
	}
	return false;

}

int ConfiguracionConfig::getVelocidad(){
	return this->nodoConfiguracion[0]["vel_personaje"].as<int>();
}

int ConfiguracionConfig::getScroll(){
	return this->nodoConfiguracion[1]["margen_scroll"].as<int>();
}
