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
}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

void Button::render(Renderer *renderer){
	SDL_Rect rect = {this->x,this->y,this->width,this->height};
//	SDL_Color color = {0x99, 0xFF, 0x00, 0xFF};
//	renderer->draw(rect,color);
	renderer->drawActionButtonWithNameInRect("boton-casa",rect);
}

void Button::pressed(RendererInteractionDelegate *delegate){
	delegate->createEntityButtonPressed("aeropuerto");
}
