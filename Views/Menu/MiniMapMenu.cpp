/*
 * MiniMapMenu.cpp
 *
 *  Created on: Oct 4, 2015
 *      Author: kaoru
 */

#include "MiniMapMenu.h"
#include "../Renderer.h"
#include "MiniMapView.h"

MiniMapMenu::MiniMapMenu(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

MiniMapMenu::~MiniMapMenu() {
	this->minimapViews.clear();
}

void MiniMapMenu::render(Renderer* renderer) {
	SDL_Rect rect = {this->x,this->y,this->width,this->height};
	SDL_Color color = {0,0,0};
	renderer->draw(rect,color);
}

void MiniMapMenu::addView(View* view){
	SDL_Rect rect = {this->x,this->y,this->width,this->height};
	MiniMapView *minimapView = new MiniMapView(view,rect);
	this->minimapViews.push_back(minimapView);
}

