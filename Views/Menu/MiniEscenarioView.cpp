/*
 * MiniEscenarioView.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#include "MiniEscenarioView.h"
#include "../Renderer.h"
#include "MiniMapView.h"
#include "MiniView.h"

MiniEscenarioView::MiniEscenarioView(MiniMapView *minimapView) {
	this->miniMapView = minimapView;
}

MiniEscenarioView::~MiniEscenarioView() {
	delete this->miniMapView;
	this->miniMapView = NULL;

	list<MiniView*>::iterator i;
	for(i=this->entitiesMiniViews.begin(); i != this->entitiesMiniViews.end(); ++i) {
		MiniView* view = *i;
		delete view;
	}
	this->entitiesMiniViews.clear();
}

MiniMapView* MiniEscenarioView::getMiniMapView() {
	return this->miniMapView;
}

void MiniEscenarioView::render(Renderer* renderer){
	//draw Tiles
	this->miniMapView->render(renderer);

	list<MiniView*>::iterator i;
	for(i=this->entitiesMiniViews.begin(); i != this->entitiesMiniViews.end(); ++i) {
		MiniView* view = *i;
		view->render(renderer);
	}
}

void MiniEscenarioView::addEntityMiniView(MiniView *miniView){
	this->entitiesMiniViews.push_back(miniView);
}

list<MiniView*>* MiniEscenarioView::getEntitiesMiniView() {
	return &(this->entitiesMiniViews);
}


