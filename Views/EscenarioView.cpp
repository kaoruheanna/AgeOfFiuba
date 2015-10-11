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
}

void EscenarioView::render(Renderer* renderer){
	this->mapView->render(renderer);
}

MapView* EscenarioView::getMapView(){
	return this->mapView;
}


