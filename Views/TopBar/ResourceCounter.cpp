/*
 * ResourceCounter.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: kaoru
 */

#include "ResourceCounter.h"
#include "../Renderer.h"
#include "../Menu/TextLabel.h"
#include <string>
using namespace std;

ResourceCounter::ResourceCounter(int x,int y,int width,int height,const char *resourceName) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->resourceName = resourceName;
	this->label = new TextLabel(x,y);
	this->setResourceValue(0);
}

ResourceCounter::~ResourceCounter() {
	delete this->label;
}

void ResourceCounter::render(Renderer* renderer) {
	SDL_Rect point = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
	renderer->draw(point,color);

	this->label->render(renderer);
}

void ResourceCounter::setResourceValue(int value) {
	string name(this->resourceName);

	string valueString;
	ostringstream convert;
	convert << value;
	valueString = convert.str();

	this->label->setMessage(name+": "+valueString);
}

const char* ResourceCounter::getResourceName() {
	return this->resourceName;
}

