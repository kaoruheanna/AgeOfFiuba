/*
 * MiniEscenarioView.cpp
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#include "MiniEscenarioView.h"
#include "../Renderer.h"
#include "MiniMapView.h"

MiniEscenarioView::MiniEscenarioView(MiniMapView *minimapView) {
	this->miniMapView = minimapView;
}

MiniEscenarioView::~MiniEscenarioView() {
	delete this->miniMapView;
	this->miniMapView = NULL;
}

MiniMapView* MiniEscenarioView::getMiniMapView() {
	return this->miniMapView;
}

void MiniEscenarioView::render(Renderer* renderer){
	//draw Tiles
	this->miniMapView->render(renderer);
}

