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
			//AGREGADO PARA QUE SEA SIEMPRE VISIBLE DESDE EL INICIO
			SDL_SetTextureColorMod(this->drawable->getTexture(),255,255,255);
			renderer->drawInMiniMap(point.x, point.y, this->drawable);
		}
	}
}
