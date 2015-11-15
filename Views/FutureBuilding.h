/*
 * FutureBuilding.h
 *
 *  Created on: Nov 11, 2015
 *      Author: kaoru
 */

#ifndef FUTUREBUILDING_H_
#define FUTUREBUILDING_H_

#include "../GlobalConstants.h"
#include <string>
using namespace std;

enum FuturePositionType {
	FuturePositionTypeAllowed = 0,
	FuturePositionTypeForbidden
};

class FutureBuilding {
public:
	FutureBuilding(string entityName,FuturePositionType positionType, LogicPosition logicPosition);
	virtual ~FutureBuilding();

	string entityName;
	FuturePositionType positionType;
	LogicPosition logicPosition;
};

#endif /* FUTUREBUILDING_H_ */
