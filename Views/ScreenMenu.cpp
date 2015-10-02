/*
 * ScreenMenu.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#include "ScreenMenu.h"
#include "Renderer.h"

ScreenMenu::ScreenMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

ScreenMenu::~ScreenMenu() {
	// TODO Auto-generated destructor stub
}

void ScreenMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xA0, 0x52, 0x2D, 0xFF};
	renderer->draw(point,color);
}

int ScreenMenu::getY(){
	return this->y;
}

