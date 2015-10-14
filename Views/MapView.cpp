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
	int width = this->model->getWidth();
	int height = this->model->getHeight();
	for (int i=0; i<width ; i++){
		for (int j=0; j<height; j++){
			SDL_Point point = this->model->getPositionForTile({ j, i });
			Uint8 red;
			Uint8 green;
			Uint8 blue;
			if (this->model->getEstado(i,j) != OCULTO){
					if (this->model->getEstado(i,j) == VISIBLE) {
						red = FOG_VISIBLE;
						blue = FOG_VISIBLE;
						green = FOG_VISIBLE;
					}
					if (this->model->getEstado(i,j) == NUBLADO) {
						red = FOG_VISITED;
						blue = FOG_VISITED;
						green = FOG_VISITED;
					}
				SDL_SetTextureColorMod( this->drawable->getTexture(), red,green,blue );
				renderer->draw(point.x, point.y, this->drawable);
			}
		}
	}
}

int MapView::getHeightInPixels(){
	int tiles = this->model->getWidth();
	int heightPerTile = this->drawable->getHeight();
	return (tiles * heightPerTile);
}



