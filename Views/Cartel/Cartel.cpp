/*
 * Cartel.cpp
 *
 *  Created on: Oct 20, 2015
 *      Author: kaoru
 */

#include "Cartel.h"
#include "../Renderer.h"
#include "../../GlobalConstants.h"
#include "../Menu/TextLabel.h"

Cartel::Cartel(int x, int y) {
	this->x = x;
	this->y = y;

	int labelX = (this->x + CARTEL_WIDTH/4);
	int labelY = (this->y + CARTEL_HEIGHT/3);
	this->label = new TextLabel(labelX,labelY);
}

Cartel::~Cartel() {
	delete this->label;
}

void Cartel::render(Renderer* renderer) {
	SDL_Rect rect = {this->x,this->y,CARTEL_WIDTH,CARTEL_HEIGHT};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(rect,color);
	this->label->render(renderer);
}

void Cartel::setMessage(string message){
	this->label->setMessage(message);
}


