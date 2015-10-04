/*
 * ScreenMenu.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#include "ScreenMenu.h"
#include "Renderer.h"
#include "StatusMenu.h"

const int STATUS_SPACING = 10;

ScreenMenu::ScreenMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->statusMenu = new StatusMenu(x+STATUS_SPACING,
									y + STATUS_SPACING,
									width/2,
									height-(2*STATUS_SPACING));
}

ScreenMenu::~ScreenMenu() {
	delete this->statusMenu;
}

void ScreenMenu::render(Renderer* renderer) {
	SDL_Rect rect = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xA0, 0x52, 0x2D, 0xFF};
	renderer->draw(rect,color);

	this->statusMenu->render(renderer);
}

int ScreenMenu::getY(){
	return this->y;
}

