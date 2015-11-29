/*
 * MobileView.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "MobileView.h"
#include <cmath>
#include "../Utils/Log.h"

MobileView::MobileView(std::string type): View(type) {
	this->model = NULL;
	this->auxDrawable = NULL;
	this->lastOrigin = this->origin;
	this->isActive = true;
}

MobileView::~MobileView() {
	this->model = NULL;
	this->auxDrawable = NULL;
}

void MobileView::setModel(MobileModel *model) {
	this->model = model;
}

void MobileView::setDrawableDeshabilitado(Drawable *drawable) {
	this->auxDrawable = drawable;
}

Drawable* MobileView::getDrawable(){
	if (this->model->isInteracting()){
		if (this->interactingDrawable != NULL){
			return this->interactingDrawable;
		}else{
			return this->drawable;
		}
	}else{
		return this->drawable;
	}
}

SDL_Point MobileView::getOrigin(){
	if (!this->model){
		return View::getOrigin();
	}
	this->lastOrigin = origin;

	int x = this->model->getX();
	int y = this->model->getY();
	SDL_Point origin = {x,y};
	this->origin = origin;
	return this->origin;
}

void MobileView::render(Renderer* renderer) {
	MotionDirection currentDirection = (this->model->isInteracting()) ? this->getInteractingDirection() : this->getMovingDirection();
	this->animationStatus = this->getDrawable()->getAnimation(currentDirection,this->model->isMoving() or this->model->isInteracting(),this->animationStatus);
	SDL_Point point = this->getOrigin();
	this->getDrawable()->animate(this->animationStatus);
	renderer->draw(point.x, point.y, this->getDrawable(),false,this->model);
}

MotionDirection MobileView::getMotionDirection(SDL_Point origen,SDL_Point destino, MotionDirection oldDirection){
	double deltaX = (destino.x - origen.x);
	double deltaY = (destino.y - origen.y);

	if ((deltaX == 0) && (deltaY == 0)){
		return oldDirection;
	}

	double angle = (atan2(deltaY,deltaX) * 180.0 / M_PI);
	if (ISOMETRIC_DRAW){
		angle += 45.0;
	}

	if (angle > 180){
		angle -= 360;
	} else if (angle < -180) {
		angle += 360;
	}

	if (angle >= 0){
		if (angle < 23){
			return EAST;
		} else if (angle < 68){
			return SOUTH_EAST;
		} else if (angle < 113){
			return SOUTH;
		} else if (angle < 158){
			return SOUTH_WEST;
		} else if (angle <= 180){
			return WEST;
		}
	}

	if (angle >= -23){
		return EAST;
	} else if (angle >= -68){
		return NORTH_EAST;
	} else if (angle >= -113){
		return NORTH;
	} else if (angle >= -158){
		return NORTH_WEST;
	} else if (angle >= -180){
		return WEST;
	}

	return oldDirection;
}

MotionDirection MobileView::getMovingDirection(){
	return this->getMotionDirection(this->lastOrigin,this->origin,this->animationStatus.direction);
}

MotionDirection MobileView::getInteractingDirection(){
	return this->getMotionDirection(this->origin,this->model->getTargetEntityPosition(),this->animationStatus.direction);
}

bool MobileView::hasModelWithId(int id) {
	return (this->model && (this->model->getId() == id));
}
