/*
 * FutureBuilding.h
 *
 *  Created on: Nov 11, 2015
 *      Author: kaoru
 */

#ifndef FUTUREBUILDING_H_
#define FUTUREBUILDING_H_

#include "View.h"
#include "../GlobalConstants.h"
#include <string>
using namespace std;

class View;

enum FuturePositionType {
	FuturePositionTypeAllowed = 0,
	FuturePositionTypeForbidden
};

class FutureBuildingView: public View {
public:
	FutureBuildingView(string type,FuturePositionType positionType, LogicPosition logicPosition);
	virtual ~FutureBuildingView();

	void setPositionType(FuturePositionType positionType);
	void setLogicPosition(LogicPosition position);

private:
	FuturePositionType positionType;
};

#endif /* FUTUREBUILDING_H_ */
