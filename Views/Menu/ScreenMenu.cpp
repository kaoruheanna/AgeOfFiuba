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
#include "ActionsMenu.h"

ScreenMenu::ScreenMenu(int width, int height) {
	this->width = width;
	this->height = height;

	int statusWidth = width/4;
	int elementsX = MENU_SPACING;
	int elementsHeight = height-(2*MENU_SPACING);

	this->statusMenu = new StatusMenu(
		elementsX,
		MENU_SPACING,
		statusWidth,
		elementsHeight
	);

	elementsX += (MENU_SPACING + statusWidth);

	this->actionsMenu = new ActionsMenu(
		elementsX,
		MENU_SPACING,
		width/3,
		elementsHeight
	);
}

ScreenMenu::~ScreenMenu() {
	delete this->statusMenu;
	delete this->actionsMenu;
}

void ScreenMenu::render(Renderer* renderer) {
	SDL_Rect rect = {0,0,this->width,this->height};
	SDL_Color color = {0xA0, 0x52, 0x2D, 0xFF};
	renderer->draw(rect,color);

	this->statusMenu->render(renderer);
	this->actionsMenu->render(renderer);
}

void ScreenMenu::setMessages(std::string firstMessage, std::string secondMessage) {
	this->statusMenu->setMessageForFirstLabel(firstMessage);
	this->statusMenu->setMessageForSecondLabel(secondMessage);
}


