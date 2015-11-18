/*
 * FutureBuilding.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: kaoru
 */

#include "FutureBuildingView.h"

FutureBuildingView::FutureBuildingView(string type,FuturePositionType positionType, LogicPosition logicPosition): View(type) {
	this->positionType = positionType;
	this->origin = {logicPosition.first,logicPosition.second};
}

FutureBuildingView::~FutureBuildingView() {
	// TODO Auto-generated destructor stub
}

FuturePositionType FutureBuildingView::getFuturePositionType(){
	return this->positionType;
}

void FutureBuildingView::setPositionType(FuturePositionType positionType) {
	this->positionType = positionType;
}

void FutureBuildingView::setLogicPosition(LogicPosition position) {
	this->origin = {position.first,position.second};
}

