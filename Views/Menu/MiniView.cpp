/*
 * MiniView.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#include "MiniView.h"
#include "../Drawable.h"
#include "../Renderer.h"
#include "../../Models/Entity.h"

MiniView::MiniView(std::string name) {
	this->drawable = NULL;
	SDL_Point origin = {0,0};
	this->origin = origin;
	this->type = name;
	this->model = NULL;
}

MiniView::~MiniView() {
	this->drawable = NULL;
}

void MiniView::setDrawable(Drawable *drawable){
	this->drawable = drawable;
}

void MiniView::setOrigin(int x, int y) {
	SDL_Point origin = {x,y};
	this->origin = origin;
}

void MiniView::render(Renderer* renderer ) {
	if (this->model){
		this->setOrigin(this->model->getPosicion().x,this->model->getPosicion().y);
	}
	renderer->drawInMiniMap(this->origin.x, this->origin.y, this->drawable);
}

std::string MiniView::getType() {
	return this->type;
}

void MiniView::setModel(Entity *model) {
	this->model = model;
}

string MiniView::NombreDrawableFromNombreTipo(string nombreTipo){
	return "mini-"+nombreTipo;
}

