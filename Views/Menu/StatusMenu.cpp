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
	this->entityNameLabel = new TextLabel(x+60,y);
	this->entityTeamLabel = new TextLabel(x+60,y+20);
	this->remainingLifeLabel = new TextLabel(x+60,y+40);
	this->specialFeatureLabel = new TextLabel(x+5,y+60);
	this->entityStatusIcon = NULL;
	this->currentEntityName = "";
	this->setStatusBlank();
}

StatusMenu::~StatusMenu() {
	this->deleteCurrentIcon();
	delete this->entityNameLabel;
	delete this->entityTeamLabel;
	delete this->remainingLifeLabel;
	delete this->specialFeatureLabel;
}

void StatusMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);



	if ((!this->entityClicked) || (!this->entityClicked->estaViva())){
		this->setStatusBlank();
	}

	this->entityStatusIcon->render(renderer);
	this->entityNameLabel->render(renderer);
	this->entityTeamLabel->render(renderer);
	this->remainingLifeLabel->render(renderer);
	this->specialFeatureLabel->render(renderer);

}


void StatusMenu::setStatusIcon() {
	this->deleteCurrentIcon();
	int wButton = 50;
	int hButton = 50;
	int xButton = ((this->x)+5);
	int yButton = ((this->y) + 5);
	Button* entityIcon = new Button(xButton, yButton, wButton, hButton);
	this->entityStatusIcon = entityIcon;
	this->entityStatusIcon->setEntityName(this->entityClicked->getNombre());

}

void StatusMenu::deleteCurrentIcon() {
	if (this->entityStatusIcon) {
		delete (this->entityStatusIcon);
		this->entityStatusIcon = NULL;
	}
}

string StatusMenu::convertIntToString(int number) {
	string Result;
	stringstream convert;
	convert << number;
	Result = convert.str();
	return Result;
}

void StatusMenu::setSpecialFeatures() {
	if (this->entityClicked->getEscudo() > 0) {
		this->specialFeatureLabel->setMessage(
				"Escudo: " + (this->convertIntToString(this->entityClicked->getEscudo())));
	} else if (this->entityClicked->getPoderAtaque() > 1) {
		this->specialFeatureLabel->setMessage(
				"Poder De Ataque: "
						+ (this->convertIntToString(this->entityClicked->getPoderAtaque())));
	} else {
		this->specialFeatureLabel->setMessage("");
	}
}

void StatusMenu::setLabels() {
	this->entityNameLabel->setMessage(this->entityClicked->getNombre());
	this->entityTeamLabel->setMessage(this->entityClicked->getTeamString());
	this->remainingLifeLabel->setMessage(
			"Vida restante: "
					+ (this->convertIntToString(this->entityClicked->getLife()))
					+ " / "
					+ (this->convertIntToString(
							this->entityClicked->getVidaInicial())));
	this->setSpecialFeatures();
}

void StatusMenu::setStatusDataForEntity(Entity* entity){
	//Si no hizo click en nada, la barra de estado debe estar vacia
	if (!entity){
		this->setStatusBlank();
		return;
	}

	//Si la entidad esta muerta limpio el menu
	if (!entity->estaViva()){
			this->setStatusBlank();
			return;
	}



	//Si clickeo algo seteo los parametros de icono y labels
	this->entityClicked = entity;
	this->setStatusIcon();
	this->setLabels();
}

void StatusMenu::setStatusBlank(){
	this->deleteCurrentIcon();
	this->entityNameLabel->setMessage("");
	this->entityTeamLabel->setMessage("");
	this->remainingLifeLabel->setMessage("");
	this->specialFeatureLabel->setMessage("");
	this->currentEntityName = "";
}

