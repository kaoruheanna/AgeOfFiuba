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
	this->auxDrawable = NULL;
}

void MobileView::setModel(MobileModel *model) {
	this->model = model;
}

void MobileView::setDrawableDeshabilitado(Drawable *drawable) {
	this->auxDrawable = drawable;
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
	MotionDirection currentDirection = this->getMotionDirection();
	this->animationStatus = this->drawable->getAnimation(currentDirection,this->model->isMoving(),this->animationStatus);
	SDL_Point point = this->getOrigin();
	this->drawable->animate(this->animationStatus);
	renderer->draw(point.x, point.y, this->drawable,false);

	//View::render(renderer);
}

// REVISAR EL TEMA CON LA ROTACION
MotionDirection MobileView::getMotionDirection() {
	double deltaX = (this->origin.x - this->lastOrigin.x);
	double deltaY = (this->origin.y - this->lastOrigin.y);

	if ((deltaX == 0) && (deltaY == 0)){
		return this->animationStatus.direction;
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

	return this->animationStatus.direction;
}
