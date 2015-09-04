/*
 * MobileView.cpp
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#include "MobileView.h"

MobileView::MobileView() {
	printf("entro al consturctor de MobileView\n");
	this->model = NULL;
}

MobileView::~MobileView() {
	// TODO Auto-generated destructor stub
}

void MobileView::setModel(MobileModel *model) {
	this->model = model;
}

SDL_Point MobileView::getOrigin(){
	if (!this->model){
//		return super()->getOrigin();
		this->origin;
	}

	int x = this->model->getX();
	int y = this->model->getY();
	SDL_Point origin = {x,y};
	return origin;
}
