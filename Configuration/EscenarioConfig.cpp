/*
 * EscenarioConfig.cpp
 *
 *  Created on: 16/09/2015
 *      Author: die89
 */

#include "EscenarioConfig.h"
#include "../Utils/Log.h"

const std::string TAG = "EscenarioConfig";

EscenarioConfig::EscenarioConfig() {
	// TODO Auto-generated constructor stub

}

EscenarioConfig::EscenarioConfig(YAML::Node nodo){
	this->nodoEscenario = nodo;
	this->validarContenido();

}

EscenarioConfig::~EscenarioConfig() {
	// TODO Auto-generated destructor stub
}

void EscenarioConfig::validarContenido(){
	if (this->nodoEscenario.IsNull() || !this->nodoEscenario.IsSequence() || this->nodoEscenario.size() != 1){
			this->cargarEscenarioPorDefecto();
			Log().Get(TAG,logDEBUG) << "No existe campo Escenario o es invalido, Se carga una configuracion por Defecto";
			return;
		}
		else{
			if (!this->verificarNombre()){
				this->nombre = "NombreDefault";
				Log().Get(TAG,logDEBUG) << "El nombre del escenario es incorrecto, se carga un nombre por defecto";
			}
			this->validarTamanio();
			Log().Get(TAG,logDEBUG) << "el nombre usado es: " << this->getNombre();// HAY QUE SACARLO LUEGO
			return;
		}
}

bool EscenarioConfig::verificarNombre(){
	if (!this->nodoEscenario[0]["nombre"] || this->nodoEscenario[0]["nombre"].IsNull() || !this->nodoEscenario[0]["nombre"].IsDefined()){
		return false;
	}
	try{
		this->nombre = this->nodoEscenario[0]["nombre"].as<std::string>();
		return true;
	}
	catch(YAML::RepresentationException& error){
		return false;
	}
}

void EscenarioConfig::validarTamanio(){
	if (!this->validarINT("size_x")){
		this->sizeX = TAMANIO_DEFAULT;
		Log().Get(TAG,logDEBUG) << "Se cargo el tamanio en X por defecto";
	}
	if (!this->validarINT("size_y")){
		this->sizeY = TAMANIO_DEFAULT;
		Log().Get(TAG,logDEBUG) << "Se cargo el tamanio en Y por defecto";
	}
	return;
}

bool EscenarioConfig::validarINT(std::string atributo){
	return false;
}

void EscenarioConfig::cargarEscenarioPorDefecto(){

}

std::list<EntidadConfig> EscenarioConfig::getEntidades() {
	std::list<EntidadConfig> lista;
	lista.push_back(EntidadConfig(10, 15, "tierra"));
	lista.push_back(EntidadConfig(15, 20, "tierra"));
	lista.push_back(EntidadConfig(10, 15, "agua"));
	lista.push_back(EntidadConfig(30, 55, "castillo"));
	return lista;
}
std::string EscenarioConfig::getNombre() {
	return (this->nombre);
}
int EscenarioConfig::getSizeX() {
	return (this->sizeX);
}
int EscenarioConfig::getSizeY() {
	return (this->sizeY);
}
EntidadConfig EscenarioConfig::getProtagonista() {
	return EntidadConfig(50,50, "juana");
}

