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
	return origin;
}

void MapView::render(Renderer* renderer ){
	int width = this -> model -> getWidth();
	int height = this -> model-> getHeight();
	for (int i=0; i<width ; i++){
		for (int j=0; j<height; j++){
			SDL_Point point = this->model->getPositionForTile({ j, i });

			//evaluar si point esta dentro de la pantalla
			renderer->draw(point.x, point.y, this->drawable, false);
		}
	}
}



