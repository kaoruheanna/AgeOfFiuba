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
		Log().Get(TAG,logDEBUG) << "No existe campo Escenario, Se carga una configuracion por Defecto";
		return;
	}


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
	return "nombre";
}
int EscenarioConfig::getSizeX() {
	return 50;
}
int EscenarioConfig::getSizeY() {
	return 50;
}
EntidadConfig EscenarioConfig::getProtagonista() {
	return EntidadConfig(50,50, "juana");
}

