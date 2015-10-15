/*
 * TopBar.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: kaoru
 */

#include "TopBar.h"
#include "../Renderer.h"

TopBar::TopBar(int width, int height) {
	this->width = width;
	this->height = height;
}

TopBar::~TopBar() {
	// TODO Auto-generated destructor stub
}

void TopBar::render(Renderer* renderer) {
	SDL_Rect rect = {0,0,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(rect,color);
}
