/*
 * EntityView.cpp
 *
 *  Created on: 14/09/2015
 *      Author: fedelonghi
 */

#include "EntityView.h"

EntityView::EntityView(std::string type): View(type) {
	this->model = NULL;
	SDL_Point pr = {0,0};
	this->pixelRef = pr;
}

EntityView::~EntityView() {
	// TODO Auto-generated destructor stub
}

void EntityView::setModel(Entity *model) {
	this->model = model;
	SDL_Point entityPosition = this->model->getPosicion(); //ojo esto esta en coordenadas logicas
	SDL_Point origin = {entityPosition.x*64 + this->pixelRef.x, entityPosition.y*32 + this->pixelRef.y};
	//aca ojo que a la posicion hay que multiplicarle el ancho y alto del tile
	this->origin = origin;
}

SDL_Point EntityView::getOrigin(){
	//esto se podria optimizar si evitamos calcularlo cada vez que lo pedimos.
	//todas las entities son estaticas?
	return this->origin;
}


