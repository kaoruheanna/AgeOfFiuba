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

SDL_Point EntityView::getOrigin(){
	//esto se podria optimizar si evitamos calcularlo cada vez que lo pedimos.
	//todas las entities son estaticas?
	SDL_Point entityPosition = this->model->getPosicion(); //ojo esto esta en coordenadas logicas
	SDL_Point origin = {entityPosition.x - this->pixelRef.x,entityPosition.y - this->pixelRef.y};
	this -> origin = origin;
	return this->origin;
}



void EntityView::render(Renderer* renderer) {
	//hace falta actualizar aca?
	SDL_Point entityPosition = this->model->getPosicion(); //ojo esto esta en coordenadas logicas
	SDL_Point origin = {entityPosition.x - this->pixelRef.x,entityPosition.y - this->pixelRef.y};
	this -> origin = origin;
	//habria que animar la entidad.

	View::render(renderer);
}


