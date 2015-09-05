/*
 * View.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "View.h"

View::View(std::string name) {
	printf("entro al consturctor de view\n");
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
	renderer->draw(point.x, point.y, this->drawable);
}

void View::setDrawable(Drawable *drawable){
	this->drawable = drawable;
}

SDL_Point View::getOrigin(){
//	printf("uso el getOrigin de View\n");
	return this->origin;
}

void View::setOrigin(int x, int y) {
	SDL_Point origin = {x,y};
	this->origin = origin;
}

std::string View::getType() {
	return this->type;
}
