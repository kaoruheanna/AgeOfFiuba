/*
 * EscenarioView.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#include "EscenarioView.h"
#include "MapView.h"
#include "Renderer.h"

EscenarioView::EscenarioView(MapView *mapView) {
	this->mapView = mapView;
}

EscenarioView::~EscenarioView() {
	delete this->mapView;
	this->mapView = NULL;

	list<View*>::iterator i;
	for(i=this->entitiesViews.begin(); i != this->entitiesViews.end(); ++i) {
		View* view = *i;
		delete (view);
	}
	this->entitiesViews.clear();
}

void EscenarioView::render(Renderer* renderer){
	//draw Tiles
	this->mapView->render(renderer);

	list<View*>::iterator i;
	for(i=this->entitiesViews.begin(); i != this->entitiesViews.end(); ++i) {
		View* view = *i;
		view->render(renderer);
	}
}

MapView* EscenarioView::getMapView(){
	return this->mapView;
}

void EscenarioView::addEntityView(View* view){
	this->entitiesViews.push_back(view);
}

list<View*>* EscenarioView::getEntitiesView() {
	return &(this->entitiesViews);
}

void EscenarioView::removeEntityViewForId(int id) {
	list<View*>::iterator it;
	for (it = this->entitiesViews.begin(); it != this->entitiesViews.end();it++){
		View *view = *it;
		if (view->hasModelWithId(id)){
			this->entitiesViews.erase(it);
			delete view;
			return;
		}
	}
}




