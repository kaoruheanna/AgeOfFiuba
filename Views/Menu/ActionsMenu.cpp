/*
 * ActionsMenu.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: kaoru
 */

#include "ActionsMenu.h"
#include "../Renderer.h"
#include "Button.h"

ActionsMenu::ActionsMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->button = new Button(x+20,y+20, 30,30);
}

ActionsMenu::~ActionsMenu() {
	delete this->button;
}

void ActionsMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);

	this->button->render(renderer);
}

void ActionsMenu::clickEvent(int x, int y, RendererInteractionDelegate *delegate) {
	if (this->isPixelInButton(x,y,this->button)){
		this->button->pressed(delegate);
	}
}

bool ActionsMenu::isPixelInButton(int x, int y, Button *button){
	if ((x < button->x) || (x > (button->x + button->width))){
		return false;
	}
	if ((y < button->y) || (y > (button->y + button->height))){
		return false;
	}
	return true;
}
