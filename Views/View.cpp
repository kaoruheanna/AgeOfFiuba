/*
 * View.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "View.h"

View::View() {
	this->texture = NULL;
	this->origin = {0,0};
}

View::~View() {
	this->texture = NULL;
}

void View::render(SDL_Renderer* sdlRenderer ) {
	this->texture->render(this->origin,sdlRenderer);
}

void View::setTexture(Texture *texture){
	this->texture = texture;
}
