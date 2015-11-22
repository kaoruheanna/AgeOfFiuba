/*
 * ActionsMenu.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: kaoru
 */

#include "ActionsMenu.h"
#include "../Renderer.h"
#include "Button.h"
#include "../../Models/Entity.h"
#include "../../Utils/Log.h"
#include "../../Utils/EscenarioSingleton.h"
#include "../../Models/User.h"

const std::string TAG = "ActionsMenu";

ActionsMenu::ActionsMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->entityID = 0;
	this->user = NULL;
}

ActionsMenu::~ActionsMenu() {
	this->deleteButtons();
}

void ActionsMenu::render(Renderer* renderer,Entity *selectedEntity) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);

	this->setButtonsForSelectedEntity(selectedEntity);

	if (this->buttons.size() <= 0){
		return;
	}

	for (list<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++){
		Button *button = *it;
		button->render(renderer);
	}
}

void ActionsMenu::clickEvent(int x, int y, RendererInteractionDelegate *delegate) {
	for (list<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++){
		Button *button = *it;
		if (button->enabled && this->isPixelInButton(x,y,button)){
			button->pressed(delegate);
		}
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

void ActionsMenu::setButtonsForSelectedEntity(Entity *selectedEntity) {
	if (!selectedEntity){
		this->deleteButtons();
		this->entityID = 0;
		return;
	}

	if (selectedEntity->getTeam() != this->user->getTeam()){
		this->deleteButtons();
		this->entityID = selectedEntity->getId();
		return;
	}

	if (selectedEntity->getId() == this->entityID){
		this->updateButtons();
		return;
	}

	this->entityID = selectedEntity->getId();
	this->deleteButtons();

	if (!selectedEntity){
		//no creo botones
		return;
	}

	int wButton = 50;
	int hButton = 50;
	int padding = 20;
	int xButton = this->x + padding;
	int yButton = this->y + (this->height/2 - hButton/2);

	list<string>::iterator it;
	for (it = selectedEntity->creables.begin();it != selectedEntity->creables.end(); it++){
		string creable = *it;
		Button *button = new Button(xButton,yButton,wButton,hButton);
		button->setEntityName(creable);

		Escenario* escenario = EscenarioSingleton::get();
		CostoConstruccion costo = escenario->factory->getCostoConstruccion(creable);
		button->enabled = this->user->puedePagar(costo);
		this->buttons.push_back(button);

		xButton += (wButton + padding);
	}
}

void ActionsMenu::updateButtons(){
	if (this->entityID == 0){
		return;
	}
	Escenario* escenario = EscenarioSingleton::get();
	for (list<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++){
		Button *button = *it;
		if (button->getEntityName() != ""){
			CostoConstruccion costo = escenario->factory->getCostoConstruccion(button->getEntityName());
			button->enabled = this->user->puedePagar(costo);
		}
	}
}

void ActionsMenu::deleteButtons(){
	for (list<Button*>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++){
		Button *button = *it;
		delete button;
	}
	this->buttons.clear();
}

void ActionsMenu::setUser(User *user){
	this->user = user;
}

