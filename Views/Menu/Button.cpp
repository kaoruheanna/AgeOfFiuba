/*
 * Button.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: kaoru
 */

#include "Button.h"
#include "../Renderer.h"

Button::Button(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->entityName = "";
}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

void Button::render(Renderer *renderer){
	if (this->entityName != ""){
		SDL_Rect rect = {this->x,this->y,this->width,this->height};
		renderer->drawActionButtonWithNameInRect("boton-"+this->entityName,rect);
	}
}

void Button::pressed(RendererInteractionDelegate *delegate){
	delegate->createEntityButtonPressed(this->entityName);
}

void Button::setEntityName(string name) {
	this->entityName = name;
}
