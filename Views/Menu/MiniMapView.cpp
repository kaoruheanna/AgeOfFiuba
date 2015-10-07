/*
 * MiniMapView.cpp
 *
 *  Created on: Oct 6, 2015
 *      Author: kaoru
 */

#include "MiniMapView.h"
#include "../View.h"
#include "../Renderer.h"

MiniMapView::MiniMapView(View* view,SDL_Rect superviewRect) {
	this->view = view;
	this->superviewRect = superviewRect;
}

MiniMapView::~MiniMapView() {
	this->view = NULL;
}

void MiniMapView::render(Renderer* renderer){

}
