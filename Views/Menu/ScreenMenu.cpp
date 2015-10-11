/*
 * ScreenMenu.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#include "ScreenMenu.h"
#include "../Renderer.h"
#include "StatusMenu.h"
//#include "MiniMapMenu.h"
#include "../../GlobalConstants.h"

ScreenMenu::ScreenMenu(int width, int height) {
	this->width = width;
	this->height = height;

	this->statusMenu = new StatusMenu(
		MENU_SPACING,
		MENU_SPACING,
		width/2,
		height-(2*MENU_SPACING)
	);

//	int minimapHeight = height-(2*MENU_SPACING);
//	int minimapWidth = minimapHeight * 2;
//	this->miniMapMenu = new MiniMapMenu(
//		width - MENU_SPACING - minimapWidth,
//		MENU_SPACING,
//		minimapWidth,
//		minimapHeight
//	);
}

ScreenMenu::~ScreenMenu() {
	delete this->statusMenu;
//	delete this->miniMapMenu;
}

void ScreenMenu::render(Renderer* renderer) {
	SDL_Rect rect = {0,0,this->width,this->height};
	SDL_Color color = {0xA0, 0x52, 0x2D, 0xFF};
	renderer->draw(rect,color);

	this->statusMenu->render(renderer);
//	this->miniMapMenu->render(renderer);
}

void ScreenMenu::addMiniMapSubview(View* view){
//	this->miniMapMenu->addView(view);
}

