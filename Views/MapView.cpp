/*
 * MapView.cpp
 *
 *  Created on: 12/09/2015
 *      Author: fedelonghi
 */

#include "MapView.h"

MapView::MapView(std::string type): View(type) {
	this->model = NULL;
}

MapView::~MapView() {
	// TODO Auto-generated destructor stub
}

void MapView::setModel(Map *model) {
	this->model = model;
}

SDL_Point MapView::getOrigin(){
	SDL_Point origin = {0,0}; //el origen sera siempre 0?
	return origin;
}

void MapView::render(Renderer* renderer ){
	// aca habria que dibujar todos los tiles visibles;

	View::render(renderer);
}



