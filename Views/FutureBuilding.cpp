/*
 * FutureBuilding.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: kaoru
 */

#include "FutureBuilding.h"

FutureBuilding::FutureBuilding(string entityName,FuturePositionType positionType, LogicPosition logicPosition) {
	this->entityName = entityName;
	this->positionType = positionType;
	this->logicPosition = logicPosition;
}

FutureBuilding::~FutureBuilding() {
	// TODO Auto-generated destructor stub
}

