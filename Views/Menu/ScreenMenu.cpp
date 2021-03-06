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
#include "../../Models/User.h"

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
		width * 0.4,
		elementsHeight
	);
}

ScreenMenu::~ScreenMenu() {
	delete this->statusMenu;
	delete this->actionsMenu;
}

void ScreenMenu::renderMenu(Renderer* renderer) {
	SDL_Rect rect = {0,0,this->width,this->height};
	SDL_Color color = {0xA0, 0x52, 0x2D, 0xFF};
	renderer->draw(rect,color);
	this->statusMenu->render(renderer);
}

void ScreenMenu::renderMenuForEntity(Renderer* renderer,std::list<Entity *> selectedEntities){
	if (selectedEntities.empty() || (selectedEntities.size()>1)){
		return;
	}
	this->actionsMenu->render(renderer,selectedEntities.front());
}

void ScreenMenu::setStatusForEntity(Entity* entity) {
	this->statusMenu->setStatusDataForEntity(entity);
}

void ScreenMenu::clickEvent(int x, int y, RendererInteractionDelegate *delegate) {
	//si no esta dentro de actionsMenu no hace nada
	this->actionsMenu->clickEvent(x,y,delegate);
}

void ScreenMenu::setUser(User *user) {
	this->actionsMenu->setUser(user);
}

void ScreenMenu::sendMousePosition(int x, int y){
	this->actionsMenu->showEntityCost(x,y);
}
