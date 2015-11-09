/*
 * TipoConfig.cpp
 *
 *  Created on: Sep 14, 2015
 *      Author: nico
 */

#include "TipoConfig.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

const std::string TAG = "TipoConfig";

TipoConfig::TipoConfig() {
	this->altoBase = 1;
	this->anchoBase = 1;
	this->delay = 0;
	this->fps = 0;
	this->imagen = "";
	this->imagenDeshabilitado = "";
	this->miniImagen = "";
	this->nombre = "";
	this->pixelRefX = (TILE_WIDTH_PIXELS/2);
	this->pixelRefY = 0;
	this->anchoFrame = 1;
	this->altoFrame = 1;
	this->minimapPixelRefType = MinimapPixelRefTypeDefault;
	this->categoria = "building";
	this->creables.clear();
}

TipoConfig::TipoConfig(YAML::Node nodo): TipoConfig(){
	// Parsea attributos YAML a items del modelo
	this->altoBase = TipoConfig::getIntAttribute(nodo, "alto_base", this->altoBase);
	this->anchoBase = TipoConfig::getIntAttribute(nodo, "ancho_base", this->anchoBase);
	this->delay = TipoConfig::getIntAttribute(nodo, "delay", this->delay);
	this->fps = TipoConfig::getFloatAttribute(nodo, "fps", this->fps);
	this->pixelRefX = TipoConfig::getIntAttribute(nodo, "pixel_ref_x", this->pixelRefX);
	this->pixelRefY = TipoConfig::getIntAttribute(nodo, "pixel_ref_y", this->pixelRefY);
	this->anchoFrame = TipoConfig::getIntAttribute(nodo, "ancho_frame", this->anchoFrame);
	this->altoFrame = TipoConfig::getIntAttribute(nodo, "alto_frame", this->altoFrame);
	this->minimapPixelRefType = TipoConfig::getIntAttribute(nodo, "mini_map_pixel_ref_type", this->minimapPixelRefType);

	this->imagen = TipoConfig::getStringAttribute(nodo, "imagen", this->imagen);
	this->imagenDeshabilitado = TipoConfig::getStringAttribute(nodo, "imagenDeshabilitado", this->imagenDeshabilitado);
	this->miniImagen = TipoConfig::getStringAttribute(nodo, "miniImagen", this->miniImagen);
	this->nombre = TipoConfig::getStringAttribute(nodo, "nombre", this->nombre);
	this->categoria = TipoConfig::getStringAttribute(nodo, "categoria", this->categoria);
	if(this->nombre == ""){
		Log().Get(TAG,logERROR) << "Nombre no puede ser null para tipo TipoConfig";
	}
	if(this->imagen == ""){
		Log().Get(TAG,logERROR) << "Imagen no puede ser null para tipo TipoConfig";
	}
	if(this->miniImagen == ""){
		Log().Get(TAG,logERROR) << "Mini Imagen no puede ser null para tipo TipoConfig";
	}
	if(this->imagenDeshabilitado == ""){
		this->imagenDeshabilitado = this->imagen;
	}
	this->creables = this->parsearCreables(nodo);
}

list<string> TipoConfig::parsearCreables(YAML::Node nodo){
	list<string> listaDeCreables;
	YAML::Node nodoCreables;
	listaDeCreables.clear();
	if (nodo["creables"] && nodo["creables"].IsDefined() && nodo["creables"].IsSequence() && !nodo["creables"].IsNull()){
		nodoCreables = nodo["creables"];
		for (std::size_t i=0;i < nodoCreables.size();i++){
			//NO realiza verificacion alguna sobre la cadena de caracteres, toma lo que haya
			string creable = nodoCreables[i].as<string>();
			listaDeCreables.push_back(creable);
		}
	}
	return listaDeCreables;
}

int TipoConfig::getIntAttribute(YAML::Node nodo, string attributeName, int defaultValue) {
	if(!nodo[attributeName] || nodo[attributeName].IsNull() || !nodo[attributeName].IsDefined()){
//		Log().Get(TAG,logWARNING) << attributeName << " no esta definido para este tipo. Usando valor default: " << defaultValue;
	} else if(!nodo[attributeName].IsScalar()){
//		Log().Get(TAG,logWARNING) << attributeName << " no es del tipo int. Usando valor default: " << defaultValue;
	} else {
		try{
			if (nodo[attributeName].as<int>() >= 0){
				return nodo[attributeName].as<int>();
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
string TipoConfig::getStringAttribute(YAML::Node nodo, string attributeName, string defaultValue){
	if(!nodo[attributeName] || nodo[attributeName].IsNull() || !nodo[attributeName].IsDefined()){
//		Log().Get(TAG,logWARNING) << attributeName << " no esta definido para este tipo. Usando valor default: " << defaultValue;
	} else {
		try{
			return nodo[attributeName].as<string>();
		} catch(YAML::RepresentationException& error){
			Log().Get(TAG,logWARNING) << attributeName << " no se puede castear a string. Error YAML: " << error.msg << ". Usando valor default: " << defaultValue;
		}
	}
	return defaultValue;
}

float TipoConfig::getFloatAttribute(YAML::Node nodo, string attributeName, float defaultValue) {
	if(!nodo[attributeName] || nodo[attributeName].IsNull() || !nodo[attributeName].IsDefined()){
//			Log().Get(TAG,logWARNING) << attributeName << " no esta definido para este tipo. Usando valor default: " << defaultValue;
		} else if(!nodo[attributeName].IsScalar()){
//			Log().Get(TAG,logWARNING) << attributeName << " no es del tipo float. Usando valor default: " << defaultValue;
		} else {
			try{
				if (nodo[attributeName].as<float>() > 0){
					return nodo[attributeName].as<float>();
				}
				else {
//					Log().Get(TAG,logWARNING) << "El valor de fps es menor a invalido, usando valor default ";
					return defaultValue;
				}
			} catch(YAML::RepresentationException& error){
//				Log().Get(TAG,logWARNING) << attributeName << " no se puede castear a float. Error YAML: " << error.msg << ". Usando valor default: " << defaultValue;
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
string TipoConfig::getImagenDeshabilitado() {
	return this->imagenDeshabilitado;
}
string TipoConfig::getMiniImagen() {
	return this->miniImagen;
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
int TipoConfig::getMinimapPixelRefType() {
	return this->minimapPixelRefType;
}

string TipoConfig::getCategoria() {
	return this->categoria;
}

list<string> TipoConfig::getCreables(){
	return this->creables;
}
