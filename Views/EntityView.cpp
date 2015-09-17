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
	int tw = 64;
	int th = 32;
	SDL_Point origin =

	{(pX-pY)*tw,
			(pX+pY)*th};

	this->origin = origin;
}

SDL_Point EntityView::getOrigin(){
	return this->origin;
}

void EntityView::render(Renderer* renderer) {
	SDL_Point point = this->getOrigin();
	renderer->draw(point.x, point.y, this->drawable,false);
}
