/*
 * ScreenMenu.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#include "ScreenMenu.h"
#include "Renderer.h"
#include "StatusMenu.h"
#include "MiniMapMenu.h"

const int SPACING = 10;

ScreenMenu::ScreenMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->statusMenu = new StatusMenu(
		x+SPACING,
		y+SPACING,
		width/2,
		height-(2*SPACING)
	);

	int minimapWidth = width / 3;
	this->miniMapMenu = new MiniMapMenu(
		width - SPACING - minimapWidth,
		y + SPACING,
		minimapWidth,
		height-(2*SPACING)
	);
}

ScreenMenu::~ScreenMenu() {
	delete this->statusMenu;
	delete this->miniMapMenu;
}

void ScreenMenu::render(Renderer* renderer) {
	SDL_Rect rect = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xA0, 0x52, 0x2D, 0xFF};
	renderer->draw(rect,color);

	this->statusMenu->render(renderer);
	this->miniMapMenu->render(renderer);
}

int ScreenMenu::getY(){
	return this->y;
}

