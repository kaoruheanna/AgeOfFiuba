/*
 * MobileModel.h
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#ifndef MOBILEMODEL_H_
#define MOBILEMODEL_H_

class MobileModel {
public:
	MobileModel();
	virtual ~MobileModel();

	int getX();
	int getY();
	void setX(int x);
	void setY(int y);
	void setDestination(int destinationX, int destionationY);
	void updatePosition();
	bool isMoving();

private:
	int x;
	int y;
	int destinationX;
	int destinationY;
	bool moving;

	int getSpeed();
};

#endif /* MOBILEMODEL_H_ */
