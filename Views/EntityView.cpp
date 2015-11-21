/*
 * EntityView.cpp
 *
 *  Created on: 14/09/2015
 *      Author: fedelonghi
 */

#include "EntityView.h"
#include "../Utils/Log.h"

const string TAG = "EntityView";

EntityView::EntityView(std::string type): View(type) {
	this->model = NULL;
	SDL_Point pr = {0,0};
	this->pixelRef = pr;
}

EntityView::~EntityView() {
	// TODO destruir view.
	this->model = NULL;

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

Drawable* EntityView::getDrawable(){
	if (this->model->isInteracting()){
		return this->drawable;
	}else{
		return this->drawable;
	}
}

void EntityView::render(Renderer* renderer) {
	SDL_Point point = this->getOrigin();
	this->animationStatus = this->drawable->getAnimation(this->animationStatus.direction,true,false,this->animationStatus);
	this->getDrawable()->animate(this->animationStatus);
	renderer->draw(point.x, point.y, this->getDrawable(), this->model->admiteNublado());
}

bool EntityView::hasModelWithId(int id) {
	return (this->model && (this->model->getId() == id));
}

