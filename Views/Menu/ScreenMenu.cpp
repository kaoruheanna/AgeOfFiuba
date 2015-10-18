/*
 * ScreenMenu.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: kaoru
 */

#include "ScreenMenu.h"
#include "../Renderer.h"
#include "StatusMenu.h"
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
}

ScreenMenu::~ScreenMenu() {
	delete this->statusMenu;
}

void ScreenMenu::render(Renderer* renderer) {
	SDL_Rect rect = {0,0,this->width,this->height};
	SDL_Color color = {0xA0, 0x52, 0x2D, 0xFF};
	renderer->draw(rect,color);

	this->statusMenu->render(renderer);
}

void ScreenMenu::setMessages(std::string firstMessage, std::string secondMessage) {
	this->statusMenu->setMessageForFirstLabel(firstMessage);
	this->statusMenu->setMessageForSecondLabel(secondMessage);
}


