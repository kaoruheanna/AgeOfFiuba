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
	this->lastOrigin = this->origin;
	this->lastDirection = SOUTH;
}

MobileView::~MobileView() {
	// TODO Auto-generated destructor stub
}

void MobileView::setModel(MobileModel *model) {
	this->model = model;
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
	this->lastDirection = this->getMotionDirection();
	this->drawable->selectAnimation(this->lastDirection,this->model->isMoving());
	View::render(renderer);
}

// REVISAR EL TEMA CON LA ROTACION
MotionDirection MobileView::getMotionDirection() {
	double deltaX = (this->origin.x - this->lastOrigin.x);
	double deltaY = (this->origin.y - this->lastOrigin.y);

	if ((deltaX == 0) && (deltaY == 0)){
		return this->lastDirection;
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

	return this->lastDirection;
}
