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
}

View::~View() {
	this->drawable = NULL;
}

void View::render(Renderer* renderer ) {
	SDL_Point point = this->getOrigin();
	this->drawable->animate();
	renderer->draw(point.x, point.y, this->drawable,true);
}

void View::render(Renderer* renderer, bool iso) {
	SDL_Point point = this->getOrigin();
	this->drawable->animate();
	renderer->draw(point.x, point.y, this->drawable, iso);
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
