/*
 * View.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "View.h"

View::View() {
	printf("entro al consturctor de view\n");
	this->texture = NULL;
	SDL_Point origin = {0,0};
	this->origin = origin;
}

View::~View() {
	this->texture = NULL;
}

void View::render(SDL_Renderer* sdlRenderer ) {
	SDL_Point point = this->getOrigin();
	this->texture->render(point,sdlRenderer);
}

void View::setTexture(Texture *texture){
	this->texture = texture;
}

SDL_Point View::getOrigin(){
//	printf("uso el getOrigin de View\n");
	return this->origin;
}

void View::setOrigin(int x, int y) {
	SDL_Point origin = {x,y};
	this->origin = origin;
}
