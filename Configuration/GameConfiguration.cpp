/*
 * GameConfiguration.cpp
 *
 *  Created on: 08/09/2015
 *      Author: die89
 */

#include "GameConfiguration.h"
#include "../Utils/Log.h"
#include "../GlobalConstants.h"

const std::string TAG = "GameConfiguration";

GameConfiguration::GameConfiguration(const string archivoAParsear){
	this->tipos.clear();
	this->parseYAML(archivoAParsear);
}

GameConfiguration::GameConfiguration(){
	this->tipos.clear();
	this->parseYAML(CONFIG_DEFAULT);
}

const std::string GameConfiguration::getPath() {
	return this->path;
}

GameConfiguration::~GameConfiguration(){
}

int GameConfiguration::getPantallaAlto(){
	return this->pantalla.getAlto();
}

int GameConfiguration::getPantallaAncho(){
	return this->pantalla.getAncho();
}

int GameConfiguration::getVelocidadPersonaje(){
	return this->configuracion.getVelocidad();
}

int GameConfiguration::getMargenScroll(){
	return this->configuracion.getScroll();
}

std::string GameConfiguration::getNombreEscenario(){
	return this->escenario.getNombre();
}

int GameConfiguration::getTamanioX(){
	return this->escenario.getSizeX();
}

int GameConfiguration::getTamanioY(){
	return this->escenario.getSizeY();
}

//ESTA FUNCION VA A COMPROBAR QUE EL ARCHIVO RESPETE LAS ESPECIFICACIONES DE YAML, NO VERIFICA LA VALIDACION DE VALORES
void GameConfiguration::loadFile(const string archivoAParsear){
	this->path = archivoAParsear;
	try{
	this->nodoRaiz = YAML::LoadFile(archivoAParsear);
		if (!nodoRaiz["pantalla"] && !nodoRaiz["configuracion"] && !nodoRaiz["tipos"] && !nodoRaiz["esceario"]){
			this->nodoRaiz = YAML::LoadFile(CONFIG_DEFAULT);
			this->path = CONFIG_DEFAULT;
			Log().Get(TAG,logERROR) << "El archivo no tiene ninguno de los campos necesarios";
		}
	}
	catch ( YAML::Exception& error){
		this->nodoRaiz = YAML::LoadFile(CONFIG_DEFAULT);
		this->path = CONFIG_DEFAULT;
		Log().Get(TAG,logWARNING) << "El archivo indicado como parametro no existe o no respeta la sintaxis de YAML, se carga el archivo por defecto";
	}
}


void GameConfiguration::parseYAML(const string archivoAParsear){
	//VERIFICA SI ARCHIVO ESTA CORRUPTO O TIENE FORMATO VALIDO DE YAML, EN ESE CASO PARSEA DIRECTAMENTE EL ARCHIVO POR DEFECTO
	this->loadFile(archivoAParsear);

	this->pantalla = PantallaConfig(this->nodoRaiz["pantalla"]);
	this->configuracion = ConfiguracionConfig(this->nodoRaiz["configuracion"]);
	YAML::Node nodoTipos = this->nodoRaiz["tipos"];
	this->escenario = EscenarioConfig(this->nodoRaiz["escenario"]);


	if(!nodoTipos.IsSequence()){
		Log().Get(TAG,logERROR) << "Nodo tipos tiene que ser una secuencia";
	} else {
		for (std::size_t i=0;i < nodoTipos.size();i++) {
		  TipoConfig* newNodo = new TipoConfig(nodoTipos[i]);
		  tipos.push_back(*newNodo);
		}
	}
}

list<TipoConfig> GameConfiguration::getTipos() {
	return this->tipos;
}

EscenarioConfig GameConfiguration::getEscenario() {
	return this->escenario;
}
