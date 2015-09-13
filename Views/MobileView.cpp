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
	this->drawable->selectAnimation(this->getMotionDirection());
	View::render(renderer);
}

// REVISAR EL TEMA CON LA ROTACION
MotionDirection MobileView::getMotionDirection() {
	double deltaX = (this->origin.x - this->lastOrigin.x);
	double deltaY = (this->origin.y - this->lastOrigin.y);

	if ((deltaX == 0) && (deltaY == 0)){
		return SOUTH;
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
//			Log().Get(logINFO) << "direccion ESTE";
			return EAST;
		} else if (angle < 68){
//			Log().Get(logINFO) << "direccion SUDESTE";
			return SOUTH_EAST;
		} else if (angle < 113){
//			Log().Get(logINFO) << "direccion sur";
			return SOUTH;
		} else if (angle < 158){
//			Log().Get(logINFO) << "direccion sudoeste";
			return SOUTH_WEST;
		} else if (angle <= 180){
//			Log().Get(logINFO) << "direccion oeste";
			return WEST;
		}
	}

	if (angle >= -23){
//		Log().Get(logINFO) << "----direccion ESTE";
		return EAST;
	} else if (angle >= -68){
//		Log().Get(logINFO) << "----direccion NORESTE";
		return NORTH_EAST;
	} else if (angle >= -113){
//		Log().Get(logINFO) << "-----direccion norte";
		return NORTH;
	} else if (angle >= -158){
//		Log().Get(logINFO) << "-----direccion noroeste";
		return NORTH_WEST;
	} else if (angle >= -180){
//		Log().Get(logINFO) << "----direccion oeste";
		return WEST;
	}

	return SOUTH;
}
