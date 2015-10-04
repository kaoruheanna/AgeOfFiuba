/*
 * StatusMenu.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: kaoru
 */

#include "StatusMenu.h"
#include "Renderer.h"
#include "../Utils/Log.h"
#include "TextLabel.h"

const std::string TAG = "StatusMenu";

StatusMenu::StatusMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->label = new TextLabel(x+10,y+10);
}

StatusMenu::~StatusMenu() {
	delete this->label;
}

void StatusMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);

	this->label->render(renderer);
}

