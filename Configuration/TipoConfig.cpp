/*
 * TipoConfig.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: nico
 */

#include "TipoConfig.h"
#include "../Utils/Log.h"

const std::string TAG = "TipoConfig";

TipoConfig::TipoConfig() {
	this->altoBase = 1;
	this->anchoBase = 1;
	this->delay = 0;
	this->fps = 0;
	this->imagen = "";
	this->nombre = "";
	this->pixelRefX = 0;
	this->pixelRefY = 0;
	this->anchoFrame = 1;
	this->altoFrame = 1;
}

TipoConfig::TipoConfig(YAML::Node nodo): TipoConfig(){
	// Parsea attributos YAML a items del modelo
	this->altoBase = TipoConfig::getIntAttribute(nodo, "alto_base", this->altoBase);
	this->anchoBase = TipoConfig::getIntAttribute(nodo, "ancho_base", this->anchoBase);
	this->delay = TipoConfig::getIntAttribute(nodo, "delay", this->delay);
	this->fps = TipoConfig::getIntAttribute(nodo, "fps", this->fps);
	this->pixelRefX = TipoConfig::getIntAttribute(nodo, "pixel_ref_x", this->pixelRefX);
	this->pixelRefY = TipoConfig::getIntAttribute(nodo, "pixel_ref_y", this->pixelRefY);
	this->anchoFrame = TipoConfig::getIntAttribute(nodo, "ancho_frame", this->anchoFrame);
	this->altoFrame = TipoConfig::getIntAttribute(nodo, "alto_frame", this->altoFrame);

	this->imagen = TipoConfig::getStringAttribute(nodo, "imagen", this->imagen);
	this->nombre = TipoConfig::getStringAttribute(nodo, "nombre", this->nombre);
	if(this->nombre == ""){
		Log().Get(TAG,logERROR) << "Nombre no puede ser null para tipo TipoConfig";
	}
	if(this->imagen == ""){
		Log().Get(TAG,logERROR) << "Imagen no puede ser null para tipo TipoConfig";
	}
}

int TipoConfig::getIntAttribute(YAML::Node nodo, string attributeName, int defaultValue) {
	if(!nodo[attributeName] || nodo[attributeName].IsNull() || !nodo[attributeName].IsDefined()){
		Log().Get(TAG,logDEBUG) << attributeName << " no esta definido para este tipo. Usando valor default: " << defaultValue;
	} else if(!nodo[attributeName].IsScalar()){
		Log().Get(TAG,logDEBUG) << attributeName << " no es del tipo int. Usando valor default: " << defaultValue;
	} else {
		try{
			return nodo[attributeName].as<int>();
		} catch(YAML::RepresentationException& error){
			Log().Get(TAG,logDEBUG) << attributeName << " no se puede castear a int. Error YAML: " << error.msg << ". Usando valor default: " << defaultValue;
		}
	}
	return defaultValue;
}
string TipoConfig::getStringAttribute(YAML::Node nodo, string attributeName, string defaultValue){
	if(!nodo[attributeName] || nodo[attributeName].IsNull() || !nodo[attributeName].IsDefined()){
		Log().Get(TAG,logDEBUG) << attributeName << " no esta definido para este tipo. Usando valor default: " << defaultValue;
	} else {
		try{
			return nodo[attributeName].as<string>();
		} catch(YAML::RepresentationException& error){
			Log().Get(TAG,logDEBUG) << attributeName << " no se puede castear a string. Error YAML: " << error.msg << ". Usando valor default: " << defaultValue;
		}
	}
	return defaultValue;
}

TipoConfig::~TipoConfig() {
}

string TipoConfig::getNombre() {
	return this->nombre;
}
string TipoConfig::getImagen() {
	return this->imagen;
}
int TipoConfig::getAnchoBase() {
	return this->anchoBase;
}
int TipoConfig::getAltoBase() {
	return this->altoBase;
}
int TipoConfig::getPixelRefX() {
	return this->pixelRefX;
}
int TipoConfig::getPixelRefY() {
	return this->pixelRefY;
}
int TipoConfig::getFPS() {
	return this->fps;
}
int TipoConfig::getDelay() {
	return this->delay;
}

int TipoConfig::getAltoFrame() {
	return this->altoFrame;
}
int TipoConfig::getAnchoFrame() {
	return this->anchoFrame;
}

