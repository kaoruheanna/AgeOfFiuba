/*
 * ConfiguracionConfig.cpp
 *
 *  Created on: 13/09/2015
 *      Author: die89
 */

#include "ConfiguracionConfig.h"
#include "../Utils/Log.h"

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
		if (this->nodoConfiguracion.IsSequence() && this->nodoConfiguracion.size() == 2){
			if (!this->verificarVelocidad()){
				Log().Get(logDEBUG) << "La velocidad del personaje es invalida, se usa el valor por defecto";
				this->nodoConfiguracion[0]["vel_personaje"] = VELOCIDAD_PERSONAJE_DEFAULT;
			}
			if (!this->verificarScroll()){
				Log().Get(logDEBUG) << "Margen de scroll invalido, se utiliza valor por defecto";
				this->nodoConfiguracion[1]["margen_scroll"] = MARGEN_SCROLL_DEFAULT;
			}
		}
}

bool ConfiguracionConfig::verificarVelocidad(){
	if (this->nodoConfiguracion[0]["vel_personaje"] && this->nodoConfiguracion[0]["vel_personaje"].IsScalar() ){
			try{
				if (this->nodoConfiguracion[0]["vel_personaje"].as<int>() > 0){
					return true;
				}
			}catch(YAML::RepresentationException& error){
				return false;
			}
	}
	return false;
}

bool ConfiguracionConfig::verificarScroll(){
	if (this->nodoConfiguracion[1]["margen_scroll"] && this->nodoConfiguracion[1]["margen_scroll"].IsScalar()){
		try{
			if(this->nodoConfiguracion[1]["margen_scroll"].as<int>() > 0){
				return true;
			}
		}catch(YAML::RepresentationException& error){
			return false;
		}
	}
	return false;
}

int ConfiguracionConfig::getVelocidad(){
	return this->nodoConfiguracion[0]["vel_personaje"].as<int>();
}

int ConfiguracionConfig::getScroll(){
	return this->nodoConfiguracion[1]["margen_scroll"].as<int>();
}
