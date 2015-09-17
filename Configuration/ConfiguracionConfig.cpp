/*
 * ConfiguracionConfig.cpp
 *
 *  Created on: 13/09/2015
 *      Author: die89
 */

#include "ConfiguracionConfig.h"
#include "../Utils/Log.h"

const std::string TAG = "ConfigurationConfig";

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
		if (this->nodoConfiguracion.IsSequence() && this->nodoConfiguracion.size() <= 2){
			if (!this->verificarVelocidad()){
				Log().Get(TAG,logDEBUG) << "La velocidad del personaje es invalida, se usa el valor por defecto";
				this->nodoConfiguracion[0]["vel_personaje"] = VELOCIDAD_PERSONAJE_DEFAULT;
			}
			if (!this->verificarScroll()){
				Log().Get(TAG,logDEBUG) << "Margen de scroll invalido, se utiliza valor por defecto";
				this->nodoConfiguracion[1]["margen_scroll"] = MARGEN_SCROLL_DEFAULT;
			}
		}
		else{
			//AMBOS VALORES ESTAN EQUIVOCADOS O EL CAMPO ES INEXISTENTE
			Log().Get(TAG,logDEBUG) << "La velocidad del personaje es invalida, se usa el valor por defecto";
			this->nodoConfiguracion[0]["vel_personaje"] = VELOCIDAD_PERSONAJE_DEFAULT;
			Log().Get(TAG,logDEBUG) << "Margen de scroll invalido, se utiliza valor por defecto";
			this->nodoConfiguracion[1]["margen_scroll"] = MARGEN_SCROLL_DEFAULT;
		}
}

bool ConfiguracionConfig::verificarVelocidad(){
	if (this->nodoConfiguracion[0]["vel_personaje"] && this->nodoConfiguracion[0]["vel_personaje"].IsScalar() && !this->nodoConfiguracion[0]["vel_personaje"].IsNull() ){
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
	if (this->nodoConfiguracion[1]["margen_scroll"] && this->nodoConfiguracion[1]["margen_scroll"].IsScalar() && !this->nodoConfiguracion[1]["margen_scroll"].IsNull()){
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
