/*
 * MiniMapView.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: kaoru
 */

#include "MiniMapView.h"
#include "../View.h"
#include "../Renderer.h"
#include "../../Models/Map.h"

MiniMapView::MiniMapView(std::string type): MiniView(type) {
	this->model = NULL;
}

MiniMapView::~MiniMapView() {
	this->model = NULL;
}

void MiniMapView::setModel(Map *model) {
	this->model = model;
}

void MiniMapView::render(Renderer* renderer ){
	int width = this->model->getWidth();
	int height = this->model->getHeight();
	for (int i=0; i<width ; i++){
		for (int j=0; j<height; j++){
			SDL_Point point = this->model->getPositionForTile({ j, i });

			/*PERMITE QUE VAYA DESCUBRIENDO EL MINIMAPA A LA PAR DEL MAPA DEL ESCENARIO
			 *
			 * Uint8 red;
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

			SDL_SetTextureColorMod( this->drawable->getTexture(), red,green,blue );*/


			//ESTA PARA QUE SE VEA TODO DEL INICIO//
			SDL_SetTextureColorMod(this->drawable->getTexture(),255,255,255);
			renderer->drawInMiniMap(point.x, point.y, this->drawable);
		}
	}
}
//}
