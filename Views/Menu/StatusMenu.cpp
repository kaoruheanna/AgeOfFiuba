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
#include "../../Models/Entity.h"
#include "Button.h"

const std::string TAG = "StatusMenu";

StatusMenu::StatusMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->firstLabel = new TextLabel(x+10,y+60);
	this->secondLabel = new TextLabel(x+10,y+80);
	this->setMessageForFirstLabel("");
	this->setMessageForSecondLabel("");
	this->entityStatusIcon = NULL;
	this->currentEntityName = "";

}

StatusMenu::~StatusMenu() {
	delete this->firstLabel;
	delete this->secondLabel;
}

void StatusMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);

//	this->firstLabel->render(renderer);
	//this->secondLabel->render(renderer);
	if (this->entityStatusIcon){
		this->entityStatusIcon->render(renderer);
		this->firstLabel->render(renderer);
		this->secondLabel->render(renderer);
	}

}

void StatusMenu::setMessageForFirstLabel(std::string message) {
	this->firstLabel->setMessage(message);
}

void StatusMenu::setMessageForSecondLabel(std::string message) {
	this->secondLabel->setMessage(message);
}

void StatusMenu::setStatusIcon(Entity* entity) {
	this->deleteCurrentIcon();
	int wButton = 50;
	int hButton = 50;
	int xButton = (((this->x + this->width) - wButton) / 2);
	int yButton = ((this->y) + 10);
	Button* entityIcon = new Button(xButton, yButton, wButton, hButton);
	this->entityStatusIcon = entityIcon;
	this->entityStatusIcon->setEntityName(entity->getNombre());

}

void StatusMenu::deleteCurrentIcon() {
	if (this->entityStatusIcon) {
		delete (this->entityStatusIcon);
		this->entityStatusIcon = NULL;
	}
}

void StatusMenu::setStatusDataForEntity(Entity* entity){
	//Si o si le llega entidad != NULL porque lo chequea con el meteodo que llama a este
	if (!entity){
		this->setStatusBlank();
		return;
	}

	if (this->currentEntityName != entity->getNombre()){
		this->setStatusIcon(entity);
		this->currentEntityName = entity->getNombre();
	}
	std::string currentEntityTeam = entity->getTeamString();
	this->firstLabel->setMessage(this->currentEntityName + "-" + currentEntityTeam);
	int Number = entity->getLife();
	Log().Get(TAG,logWARNING) << "La vida actual de: " << entity->getNombre() << "es: " << entity->getLife();

	string Result;
	stringstream convert;
	convert << Number;
	Result = convert.str();

	this->secondLabel->setMessage("Vida restante: "+ Result);


}

void StatusMenu::setStatusBlank(){
	this->deleteCurrentIcon();
	this->firstLabel->setMessage("");
	this->secondLabel->setMessage("");
	this->currentEntityName = "";
}

