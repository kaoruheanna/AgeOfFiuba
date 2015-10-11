/*
 * EntityFactory.cpp
 *
 *  Created on: Oct 8, 2015
 *      Author: dario
 */

#include "EntityFactory.h"
#include "../Utils/Log.h"

EntityFactory::EntityFactory(Map *mundo, map<string, SDL_Point> sizeByType) {
	this->mundo = mundo;
	this->sizeByType = sizeByType;
}

EntityFactory::~EntityFactory() {}

SDL_Point EntityFactory::getPosition(const string& tipo, SDL_Point posicion) {
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
	return this->mundo->getPositionForTile(posicion);
}

SDL_Point EntityFactory::getSize(const string& tipo, SDL_Point posicion) {
	map<string, SDL_Point>::iterator found = sizeByType.find(tipo);
	SDL_Point size = { 1, 1 };
	if (found != sizeByType.end()) {
		size = found->second;
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
	return (MobileModel*)this->crearEntidad(tipo,posicion,true);
}

Entity* EntityFactory::crearEntidad(const string& tipo, SDL_Point posicion, bool esProtagonista) {
	if (tipo == "") {
		Log().Get("Escenario", logWARNING) << "La entidad tiene que tener un tipo. Descartando entidad.";
		return NULL;
	}
	SDL_Point pos = this->getPosition(tipo,posicion);
	SDL_Point size = this->getSize(tipo,posicion);

	if (esProtagonista) {
		return new MobileModel(tipo, pos, size.x,size.y);
	}

	return new Entity(tipo, pos, size.x, size.y);
}
