/*
 * MiniView.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#include "MiniView.h"
#include "../Drawable.h"
#include "../Renderer.h"
#include "../../Models/Entity.h"
#include "../../GlobalConstants.h"

MiniView::MiniView(std::string name) {
	this->drawable = NULL;
	SDL_Point origin = {0,0};
	this->origin = origin;
	this->type = name;
	this->model = NULL;
}

MiniView::~MiniView() {
	this->drawable = NULL;
}

void MiniView::setDrawable(Drawable *drawable){
	this->drawable = drawable;
}

void MiniView::setOrigin(int x, int y) {
	SDL_Point origin = {x,y};
	this->origin = origin;
}

void MiniView::render(Renderer* renderer ) {
	if (this->model){
		this->setOrigin(this->model->getPosicion().x,this->model->getPosicion().y);
	}

	int tilesInX = (this->model) ? this->model->getAnchoBase() : 1;
	int tilesInY = (this->model) ? this->model->getAltoBase() : 1;

	for (int i = 0; i < tilesInX; i++){
		for (int j = 0; j < tilesInY; j++){
			int mapPositionX = (this->origin.x + i*TILE_HEIGHT_PIXELS);
			int mapPositionY = (this->origin.y + j*TILE_HEIGHT_PIXELS);
			renderer->drawInMiniMap(mapPositionX,mapPositionY, this->drawable);
		}
	}
}

std::string MiniView::getType() {
	return this->type;
}

void MiniView::setModel(Entity *model) {
	this->model = model;
}

string MiniView::NombreDrawableFromNombreTipo(string nombreTipo){
	return "mini-"+nombreTipo;
}

