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
	this->sizeX = 0;
	this->sizeY = 0;
}

EscenarioConfig::EscenarioConfig(YAML::Node nodo){
	this->sizeX = 0;
	this->sizeY = 0;
	this->nodoEscenario = nodo;
	this->validarContenido();

}

EscenarioConfig::~EscenarioConfig() {
	// TODO Auto-generated destructor stub
}

void EscenarioConfig::validarContenido(){
	if (this->nodoEscenario.IsNull() || !this->nodoEscenario.IsSequence() || this->nodoEscenario.size() != 1){
			this->cargarEscenarioPorDefecto();
//			Log().Get(TAG,logWARNING) << "No existe campo Escenario o es invalido, Se carga una configuracion por Defecto";
			return;
		}
		else{
			if (!this->verificarNombre()){
				this->nombre = "NombreDefault";
//				Log().Get(TAG,logWARNING) << "El nombre del escenario es incorrecto, se carga un nombre por defecto";
			}
			this->validarTamanio();
			this->parsearProtagonista();
			this->parsearEntidades();

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
//		Log().Get(TAG,logWARNING) << "Tamanio X de pantalla invalido, se cargo el tamanio en X por defecto";
	}
	else{
			this->sizeX = this->nodoEscenario[0]["size_x"].as<int>();
	}
	if (!this->validarINT("size_y")){
		this->sizeY = TAMANIO_DEFAULT;
//		Log().Get(TAG,logWARNING) << "Tamanio Y de pantalla invalido, se cargo el tamanio en X por defecto";
	}
	else{
			this->sizeY = this->nodoEscenario[0]["size_y"].as<int>();
	}
	return;
}

bool EscenarioConfig::validarINT(std::string atributo){
	if (this->nodoEscenario[0][atributo].IsScalar() && !this->nodoEscenario[0].IsNull() && this->nodoEscenario[0][atributo].IsDefined()){
			try{
				if (this->nodoEscenario[0][atributo].as<int>() > 0){
					return true;
				}
				else { return false; }
			}catch(YAML::RepresentationException& error) {return false;}
		}
		return false;
}

void EscenarioConfig::parsearProtagonista(){
	EntidadConfig personajeParseado = EntidadConfig(0,0,"soldado", "NEUTRAL");
	YAML::Node nodoProtagonista = this->nodoEscenario[0]["protagonista"];
	if (nodoProtagonista.IsSequence() && nodoProtagonista.size() == 1 && !nodoProtagonista[0].IsNull()){
		try{
			personajeParseado = EntidadConfig(nodoProtagonista[0]);
		} catch(exception &e){
//				Log().Get(TAG,logERROR) << "Mal formato del protagonista";
		}
	} else {
		Log().Get(TAG,logWARNING) << "El protagonista no existe o se encuentra mal definido, se cargo el protagonista por defecto";
	}
	this->protagonista = personajeParseado;
}

void EscenarioConfig::parsearEntidades(){
	if (this->nodoEscenario[0]["entidades"] && this->nodoEscenario[0]["entidades"].IsSequence() && !this->nodoEscenario[0]["entidades"].IsNull()){
		YAML::Node nodoEntidades = this->nodoEscenario[0]["entidades"];
		for (std::size_t i=0;i < nodoEntidades.size();i++) {
			EntidadConfig aux ;//= EntidadConfig(0,0,"");
			try{
				EntidadConfig newNodo(nodoEntidades[i]);
				this->lista.push_back(newNodo);
			} catch(exception &e){
//				Log().Get(TAG,logERROR) << "Mal formato de la entidad N°: " << i;
			}
		}
	}
	else{
//		Log().Get(TAG,logERROR) << "El formato del campo entidades no es correcto o no existe";
	}
}

void EscenarioConfig::cargarEscenarioPorDefecto(){
	this->sizeX = TAMANIO_DEFAULT;
	this->sizeY = TAMANIO_DEFAULT;
}

std::list<EntidadConfig> EscenarioConfig::getEntidades() {
	return this->lista;
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
	return this->protagonista;
}
