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
	this->construccionIncompletaDrawable = NULL;
}

EntityView::~EntityView() {
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
	if (!(this->model->esProgresoCompleto()) && this->construccionIncompletaDrawable){
		return this->construccionIncompletaDrawable;
	}

	if (this->model->isInteracting()){
		return this->interactingDrawable;
	}else{
		return this->drawable;
	}
}

void EntityView::render(Renderer* renderer) {
	SDL_Point point = this->getOrigin();
	this->animationStatus = this->getDrawable()->getAnimation(this->animationStatus.direction,true,this->animationStatus);
	this->getDrawable()->animate(this->animationStatus);
	renderer->draw(point.x, point.y, this->getDrawable(), this->model->admiteNublado());
}

bool EntityView::hasModelWithId(int id) {
	return (this->model && (this->model->getId() == id));
}

bool EntityView::isBuildingView() {
	return (this->model && this->model->esBuilding());
}

void EntityView::setConstruccionIncompletaDrawable(Drawable *drawable){
	this->construccionIncompletaDrawable = drawable;
}
