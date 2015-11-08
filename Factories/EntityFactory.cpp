/*
 * EntityFactory.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: dario
 */

#include "EntityFactory.h"
#include "../Utils/Log.h"

const string TAG = "EntityFactory";
int getId(bool reset = false);

EntityFactory::EntityFactory(Map *mundo,list<TipoConfig> tiposConfigList) {
	this->mundo = mundo;
	list<TipoConfig>::iterator tipo;
	for (tipo = tiposConfigList.begin(); tipo != tiposConfigList.end(); ++tipo) {
		tipos[(*tipo).getNombre()] = *tipo;
	}
	getId(true);
}

EntityFactory::~EntityFactory() {}

SDL_Point EntityFactory::getPositionForTile(const string& tipo, SDL_Point posicion,bool centered) {
	if (posicion.x < 0 || posicion.x >= this->mundo->getWidth()) {
		posicion.x = 0;
		Log().Get("Escenario", logWARNING) << "La entidad " << tipo
				<< " esta fuera del mapa en ancho. Asumida posicion x = 0.";
	}
	if (posicion.y < 0 || posicion.y >= this->mundo->getHeight()) {
		posicion.y = 0;
		Log().Get("Escenario", logWARNING) << "La entidad " << tipo
				<< " esta fuera del mapa en alto. Asumida posicion y = 0.";
	}
	// Cambia de coordenadas tile a coordenadas mapa "pixel"
	return this->mundo->getPositionForTile(posicion,centered);
}

SDL_Point EntityFactory::getSize(const string& tipo) {
	TipoConfig tipoConfig = this->tipos[tipo];
	SDL_Point size = { 1, 1 };
	if (this->tipos.count(tipo)) {
		size = {tipoConfig.getAnchoBase(), tipoConfig.getAltoBase() };
	} else {
		Log().Get("Escenario", logWARNING) << "No se encontro el tipo "
				<< tipo << ". Usando tamaño de la base 1x1.";
	}
	if (size.x < 1) {
		size.x = 1;
		Log().Get("Escenario", logWARNING) << "Ancho de base del tipo "
				<< tipo
				<< " no puede ser menor a una unidad. Usando una unidad de ancho.";
	}
	if (size.y < 1) {
		size.y = 1;
		Log().Get("Escenario", logWARNING) << "Alto de base del tipo "
				<< tipo
				<< " no puede ser menor a una unidad. Usando una unidad de alto.";
	}
	return size;
}

MobileModel* EntityFactory::crearProtagonista(const string& tipo, SDL_Point posicion) {
	SDL_Point pos = this->getPositionForTile(tipo,posicion,(this->tipos[tipo].getCategoria() == "warrior"));
	SDL_Point size = this->getSize(tipo);

	return new MobileModel(0, tipo, pos, size.x,size.y);
}

int getId(bool reset) {
	static int id;

	if (reset) {
		id = 0;
		return id;
	}

	if (!id) {
		id = 0;
	}
	id++;
	return id;
}

Entity* EntityFactory::crearEntidad(const string& tipo, SDL_Point posicion) {
	int id = getId(false);
	if (tipo == "") {
		Log().Get("Escenario", logWARNING) << "La entidad tiene que tener un tipo. Descartando entidad.";
		return NULL;
	}
	SDL_Point pos = this->getPositionForTile(tipo,posicion,(this->tipos[tipo].getCategoria() == "warrior"));
	SDL_Point size = this->getSize(tipo);

	if (this->tipos[tipo].getCategoria() == "warrior") {
		return new MobileModel(id, tipo, pos, size.x,size.y);
	}
	if (this->tipos[tipo].getCategoria() == "worker") {
		return new MobileModel(id, tipo, pos, size.x,size.y);
	}
	if (this->tipos[tipo].getCategoria() == "resource") {
		return new Resource(id, tipo, pos, size.x, size.y);
	}
	if (this->tipos[tipo].getCategoria() == "building") {
		return new Building(id, tipo, pos, size.x,size.y);
	}

	return new Entity(id,tipo, pos, size.x, size.y);
}

Entity* EntityFactory::crearEntidad(const string& tipo, SDL_Point posicion, const string& equipo) {
	Entity* entidad = this->crearEntidad(tipo, posicion);
	if(equipo.compare("RED") == 0){
		entidad->setTeam(RED);
	} else if(equipo.compare("BLUE") == 0){
		entidad->setTeam(BLUE);
	} else if(equipo.compare("GREEN") == 0){
		entidad->setTeam(GREEN);
	} else if(equipo.compare("YELLOW") == 0){
		entidad->setTeam(YELLOW);
	} else {
		entidad->setTeam(NEUTRAL);
	}
	return entidad;
}

Resource* EntityFactory::crearRecurso(const string& tipo, SDL_Point posicion) {
	return (Resource*)this->crearEntidad(tipo,posicion);
}
