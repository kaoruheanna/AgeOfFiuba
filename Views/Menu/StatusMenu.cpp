/*
 * StatusMenu.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: kaoru
 */

#include "StatusMenu.h"
#include "../Renderer.h"
#include "../../Utils/Log.h"
#include "TextLabel.h"

const std::string TAG = "StatusMenu";

StatusMenu::StatusMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->firstLabel = new TextLabel(x+10,y+10);
	this->secondLabel = new TextLabel(x+10,y+40);
	this->setMessageForFirstLabel("");
	this->setMessageForSecondLabel("");

}

StatusMenu::~StatusMenu() {
	delete this->firstLabel;
	delete this->secondLabel;
}

void StatusMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);

	this->firstLabel->render(renderer);
	this->secondLabel->render(renderer);
}

void StatusMenu::setMessageForFirstLabel(std::string message) {
	this->firstLabel->setMessage(message);
}

void StatusMenu::setMessageForSecondLabel(std::string message) {
	this->secondLabel->setMessage(message);
}

