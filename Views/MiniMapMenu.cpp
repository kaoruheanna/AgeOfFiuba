/*
 * MiniMapMenu.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: kaoru
 */

#include "MiniMapMenu.h"
#include "Renderer.h"

MiniMapMenu::MiniMapMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

MiniMapMenu::~MiniMapMenu() {
	// TODO Auto-generated destructor stub
}

void MiniMapMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0,0,0};
	renderer->draw(point,color);
}

