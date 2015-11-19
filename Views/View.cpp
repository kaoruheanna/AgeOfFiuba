/*
 * View.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "View.h"

View::View(std::string name) {
	this->drawable = NULL;
	SDL_Point origin = {0,0};
	this->origin = origin;
	this->type = name;
	this->animationStatus.direction = MotionDirection(0);
	this->animationStatus.animationIndex = 0;
	this->animationStatus.isMoving = false;
}

View::~View() {
	this->drawable = NULL;
	this->animationStatus.animationIndex = 0;
	this->animationStatus.isMoving = NULL;
}

void View::setDrawableDeshabilitado(Drawable *drawable) {
//TODO que hay que hacer aca?
}

void View::render(Renderer* renderer ) {
	SDL_Point point = this->getOrigin();
	this->drawable->animate(this->animationStatus);
	renderer->draw(point.x, point.y, this->drawable,true);
}

void View::setDrawable(Drawable *drawable){
	this->drawable = drawable;
}

SDL_Point View::getOrigin(){
	return this->origin;
}

void View::setOrigin(int x, int y) {
	SDL_Point origin = {x,y};
	this->origin = origin;
}

std::string View::getType() {
	return this->type;
}

Drawable* View::getDrawable(){
	return this->drawable;
}
