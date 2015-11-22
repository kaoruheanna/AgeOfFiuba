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
	this->attackFeatureLabel = new TextLabel(x+5,y+60);
	this->defenseFeatureLabel = new TextLabel(x+5,y+80);
	this->constructionProgressLabel = new TextLabel(x+5,y+100);
	this->entityStatusIcon = NULL;
	this->entityClicked = NULL;
	this->currentEntityName = "";
	this->setStatusBlank();
}

StatusMenu::~StatusMenu() {
	this->deleteCurrentIcon();
	delete this->entityNameLabel;
	delete this->entityTeamLabel;
	delete this->remainingLifeLabel;
	delete this->attackFeatureLabel;
	delete this->defenseFeatureLabel;
	delete this->constructionProgressLabel;
}

void StatusMenu::actualizarVida(){
	this->remainingLifeLabel->setMessage(
				"Vida restante: "
				+ (this->convertIntToString(this->entityClicked->getLife()))
				+ " / "
				+ (this->convertIntToString(this->entityClicked->getVidaInicial())));
}

void StatusMenu::actualizarProgreso(){
	if (this->entityClicked->getProgresoConstruccion() < PROGRESO_COMPLETO){
			int currentPercentage = (((this->entityClicked->getProgresoConstruccion())*100)/PROGRESO_COMPLETO);
			this->constructionProgressLabel->setMessage("Porcentaje Terminado: "
					+ this->convertIntToString(currentPercentage)
					+ "%");
		}
		else{
			this->constructionProgressLabel->setMessage("");
		}
}

void StatusMenu::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);



	if ((this->entityClicked == NULL) || (!this->entityClicked->estaViva())){
		this->setStatusBlank();
		this->entityClicked = NULL;
		return;
	}
	else if (this->entityStatusIcon){
		this->entityStatusIcon->render(renderer);
		this->entityNameLabel->render(renderer);
		this->entityTeamLabel->render(renderer);
		this->actualizarVida();
		this->remainingLifeLabel->render(renderer);
		this->attackFeatureLabel->render(renderer);
		this->defenseFeatureLabel->render(renderer);
		this->actualizarProgreso();
		this->constructionProgressLabel->render(renderer);
	}
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

	//FEATURE PODER DE ATAQUE Y ESCUDO
	/*
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
*/
	this->attackFeatureLabel->setMessage(
					"Poder De Ataque: " +
					(this->convertIntToString(this->entityClicked->getPoderAtaque())) +
					" Alcance: " +
					(this->convertIntToString(this->entityClicked->getAlcance())));
	this->defenseFeatureLabel->setMessage(
						"Escudo: " +
						(this->convertIntToString(this->entityClicked->getEscudo())) +
						" Escudo a Distancia: " +
						(this->convertIntToString(this->entityClicked->getEscudoDistancia())));
	//FEATURE POGRESO DE LA CONSTRUCCION
	this->actualizarProgreso();
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
	this->entityClicked = NULL;
	this->entityNameLabel->setMessage("");
	this->entityTeamLabel->setMessage("");
	this->remainingLifeLabel->setMessage("");
	this->attackFeatureLabel->setMessage("");
	this->defenseFeatureLabel->setMessage("");
	this->constructionProgressLabel->setMessage("");
	this->currentEntityName = "";
}

