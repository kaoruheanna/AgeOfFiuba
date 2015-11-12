/*
 * EntidadConfig.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: nico
 */

#include "EntidadConfig.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

const std::string TAG = "EntidadConfig";

EntidadConfig::EntidadConfig(){
	this->x = 0;
	this->y = 0;
	this->tipo = "";
	this->equipo = NOMBRE_EQUIPO_NEUTRAL;
}

EntidadConfig::EntidadConfig(YAML::Node nodo): EntidadConfig(){
	this->x = EntidadConfig::getIntAttribute(nodo, "x", this->x);
	this->y = EntidadConfig::getIntAttribute(nodo, "y", this->y);

	this->tipo = EntidadConfig::getStringAttribute(nodo, "tipo", this->tipo);
	this->equipo = EntidadConfig::getStringAttribute(nodo, "equipo", this->equipo);

	if(this->tipo == ""){
		throw runtime_error("Tipo no puede ser vacio.");
	}
}

EntidadConfig::EntidadConfig(int x, int y, std::string tipo, std::string equipo) {
	this->x = x;
	this->y = y;
	this->tipo = tipo;
	this->equipo = equipo;
}

EntidadConfig::~EntidadConfig() { }

int EntidadConfig::getX() {
	return this->x;
}

int EntidadConfig::getY() {
	return this->y;
}

string EntidadConfig::getTipo() {
	return this->tipo;
}

string EntidadConfig::getEquipo() {
	return this->equipo;
}

// Helpers de parseo
YAML::Node EntidadConfig::findNodeByName(YAML::Node nodo, string attributeName) {
	YAML::Node toReturn = nodo[attributeName];
	if(!toReturn || toReturn.IsNull()){
		for(int index = 0; index < nodo.size(); index++){
			toReturn = nodo[index][attributeName];
			if(toReturn && !toReturn.IsNull()){
				index = nodo.size();
			}
		}
	}
	return toReturn;
}

int EntidadConfig::getIntAttribute(YAML::Node nodo, string attributeName, int defaultValue) {
	YAML::Node attribute = EntidadConfig::findNodeByName(nodo, attributeName);
	if(!attribute || attribute.IsNull() || !attribute.IsDefined()){
//		Log().Get(TAG,logWARNING) << attributeName << " no esta definido para este tipo. Usando valor default: " << defaultValue;
	} else if(!attribute.IsScalar()){
//		Log().Get(TAG,logWARNING) << attributeName << " no es del tipo int. Usando valor default: " << defaultValue;
	} else {
		try{
			if (attribute.as<int>() >= 0){
				return attribute.as<int>();
			}else{
//				Log().Get(TAG,logERROR) << "El atributo: " << attributeName << " es negativo";
				return defaultValue;
			}
		} catch(YAML::RepresentationException& error){
//			Log().Get(TAG,logWARNING) << attributeName << " no se puede castear a int. Error YAML: " << error.msg << ". Usando valor default: " << defaultValue;
		}
	}
	return defaultValue;
}

string EntidadConfig::getStringAttribute(YAML::Node nodo, string attributeName, string defaultValue){
	YAML::Node attribute = EntidadConfig::findNodeByName(nodo, attributeName);
	if(!attribute || attribute.IsNull() || !attribute.IsDefined()){
//		Log().Get(TAG,logWARNING) << attributeName << " no esta definido para este tipo. Usando valor default: " << defaultValue;
	} else {
		try{
			return attribute.as<string>();
		} catch(YAML::RepresentationException& error){
			// Log().Get(TAG,logWARNING) << attributeName << " no se puede castear a string. Error YAML: " << error.msg << ". Usando valor default: " << defaultValue;
		}
	}
	return defaultValue;
}
