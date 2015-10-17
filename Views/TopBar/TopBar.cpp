/*
 * TopBar.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: kaoru
 */

#include "TopBar.h"
#include "../Renderer.h"
#include "ResourceCounter.h"

TopBar::TopBar(int width, int height) {
	this->width = width;
	this->height = height;

	int verticalSpacing = 5;
	int counterHeight = height - (2*verticalSpacing);

	int counterWidth = (width/4);
	int horizontalSpacing = 20;
	int resourceX = horizontalSpacing;

	this->firstResourceCounter = new ResourceCounter(resourceX,verticalSpacing,counterWidth,counterHeight,(char*)"Madera");

	resourceX += (counterWidth + horizontalSpacing);
	this->secondResourceCounter = new ResourceCounter(resourceX,verticalSpacing,counterWidth,counterHeight,(char*)"Comida");

	resourceX += (counterWidth + horizontalSpacing);
	this->thirdResourceCounter = new ResourceCounter(resourceX,verticalSpacing,counterWidth,counterHeight,(char*)"Piedra");
}

TopBar::~TopBar() {
	delete this->firstResourceCounter;
	delete this->secondResourceCounter;
	delete this->thirdResourceCounter;
}

void TopBar::render(Renderer* renderer) {
	SDL_Rect rect = {0,0,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(rect,color);

	this->firstResourceCounter->render(renderer);
	this->secondResourceCounter->render(renderer);
	this->thirdResourceCounter->render(renderer);
}
