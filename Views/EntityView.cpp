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
	int pX = this->model->getPosicion().x; //ojo esto esta en coordenadas logicas
	int pY = this->model->getPosicion().y;
	SDL_Point origin =	{pX,pY};
	this->origin = origin;
}

SDL_Point EntityView::getOrigin(){
	return this->model->getPosicion();
}

void EntityView::render(Renderer* renderer) {
	SDL_Point point = this->getOrigin();
	Uint8 red;
	Uint8 green;
	Uint8 blue;
	if (this->model->getEstado() != OCULTO){
		if (this->model->getEstado() == VISIBLE) {
			red = FOG_VISIBLE;
			blue = FOG_VISIBLE;
			green = FOG_VISIBLE;
		}
		if (this->model->getEstado() == NUBLADO) {
			red = FOG_VISITED;
			blue = FOG_VISITED;
			green = FOG_VISITED;
		}
		this->animationStatus = this->drawable->getAnimation(this->animationStatus.direction,true,this->animationStatus);
		this->drawable->animate(this->animationStatus);
		SDL_SetTextureColorMod( this->drawable->getTexture(), red,green,blue );
		renderer->draw(point.x, point.y, this->drawable);
	}
}

