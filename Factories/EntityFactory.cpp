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
		Log().Get(TAG, logWARNING) << "La entidad " << tipo
				<< " esta fuera del mapa en ancho. Asumida posicion x = 0.";
	}
	if (posicion.y < 0 || posicion.y >= this->mundo->getHeight()) {
		posicion.y = 0;
		Log().Get(TAG, logWARNING) << "La entidad " << tipo
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
		Log().Get(TAG, logWARNING) << "No se encontro el tipo "
				<< tipo << ". Usando tamaño de la base 1x1.";
	}
	if (size.x < 1) {
		size.x = 1;
		Log().Get(TAG, logWARNING) << "Ancho de base del tipo "
				<< tipo
				<< " no puede ser menor a una unidad. Usando una unidad de ancho.";
	}
	if (size.y < 1) {
		size.y = 1;
		Log().Get(TAG, logWARNING) << "Alto de base del tipo "
				<< tipo
				<< " no puede ser menor a una unidad. Usando una unidad de alto.";
	}
	return size;
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

Entity* EntityFactory::crearEntidad(const string& tipo, SDL_Point tile, bool aumentarID) {
	int id = (aumentarID) ? getId(false) : 0;
	if (tipo == "") {
		Log().Get(TAG, logWARNING) << "La entidad tiene que tener un tipo. Descartando entidad.";
		return NULL;
	}
	SDL_Point pos = this->getPositionForTile(tipo,tile,(this->tipos[tipo].getCategoria() == "warrior"));
	SDL_Point size = this->getSize(tipo);

	Entity* returnEntity = NULL;
	if (this->tipos[tipo].getCategoria() == "warrior") {
		returnEntity = new Warrior(id, tipo, pos, size.x,size.y);
	}
	if (this->tipos[tipo].getCategoria() == "worker") {
		returnEntity = new Worker(id, tipo, pos, size.x,size.y);
	}
	if (this->tipos[tipo].getCategoria() == "resource") {
		returnEntity = new Resource(id, tipo, pos, size.x, size.y);
	}
	if (this->tipos[tipo].getCategoria() == "building") {
		returnEntity = new Building(id, tipo, pos, size.x,size.y);
	}

	if (returnEntity) {
		returnEntity->setPropiedadesTipoUnidad(this->tipos[tipo].getPropiedadesTipoUnidad());
		return returnEntity;
	}

	Log().Get(TAG) << "Creo entity desde EntityFactory";
	return new Entity(id,tipo, pos, size.x, size.y);
}

Entity* EntityFactory::crearEntidad(const string& tipo, SDL_Point tile, const string& equipo, bool aumentarID) {
	Entity* entidad = this->crearEntidad(tipo, tile,aumentarID);
	if(equipo.compare(NOMBRE_EQUIPO_RED) == 0){
		entidad->setTeam(TEAM_RED);

	} else if(equipo.compare(NOMBRE_EQUIPO_BLUE) == 0){
		entidad->setTeam(TEAM_BLUE);

	} else if(equipo.compare(NOMBRE_EQUIPO_GREEN) == 0){
		entidad->setTeam(TEAM_GREEN);

	} else if(equipo.compare(NOMBRE_EQUIPO_YELLOW) == 0){
		entidad->setTeam(TEAM_YELLOW);

	} else {
		entidad->setTeam(TEAM_NEUTRAL);
	}
	return entidad;
}

Entity* EntityFactory::crearEntidadParaConstruir(const string& tipo, SDL_Point posicion, const string& equipo) {
	return this->crearEntidad(tipo,posicion,equipo,false);
}

Resource* EntityFactory::crearRecurso(const string& tipo, SDL_Point posicion) {
	return (Resource*)this->crearEntidad(tipo,posicion);
}

bool EntityFactory::esBuilding(const string& tipo) {
	if (tipo == ""){
		return false;
	}

	return (this->tipos[tipo].getCategoria() == "building");
}

bool EntityFactory::esMobileModel(const string& tipo) {
	if (tipo == ""){
		return false;
	}
	string categoria = this->tipos[tipo].getCategoria();

	return ((categoria == "warrior") || (categoria == "worker"));
}
