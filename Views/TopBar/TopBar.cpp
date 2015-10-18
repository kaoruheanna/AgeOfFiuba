/*
 * TopBar.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: kaoru
 */

#include "TopBar.h"
#include "../Renderer.h"
#include "ResourceCounter.h"
#include "../../Utils/Log.h"

const std::string TAG = "TopBar";


TopBar::TopBar(int width, int height) {
	this->width = width;
	this->height = height;
	this->protagonista = NULL;
}

TopBar::~TopBar() {
	for (list<ResourceCounter*>::iterator it = this->resourcesCounters.begin(); it != this->resourcesCounters.end(); it++){
		ResourceCounter *counter = *it;
		delete counter;
	}
}

void TopBar::render(Renderer* renderer) {
	SDL_Rect rect = {0,0,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(rect,color);

	for (list<ResourceCounter*>::iterator it = this->resourcesCounters.begin(); it != this->resourcesCounters.end(); it++){
		ResourceCounter *counter = *it;
		string resourceName = counter->getResourceName();
		int value = this->protagonista->getValueForResource(resourceName);
		counter->setResourceValue(value);
		counter->render(renderer);
	}
}

void TopBar::setProtagonista(MobileModel *protagonista) {
	this->protagonista = protagonista;

	if (!this->resourcesCounters.empty()){
		//ya cree los counters
		return;
	}

	list<string> names = this->protagonista->getResourcesNames();

	int verticalSpacing = 5;
	int counterHeight = height - (2*verticalSpacing);

	int counterWidth = (width/4);
	int horizontalSpacing = 20;
	int resourceX = horizontalSpacing;

	for (list<string>::iterator it = names.begin(); it != names.end(); it++){
		string name = *it;
		ResourceCounter *counter = new ResourceCounter(resourceX,verticalSpacing,counterWidth,counterHeight,name);
		this->resourcesCounters.push_back(counter);
		resourceX += (counterWidth + horizontalSpacing);
	}
}
