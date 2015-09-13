/*
 * MobileView.h
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#ifndef MOBILEVIEW_H_
#define MOBILEVIEW_H_

#include "View.h"
#include "../Models/MobileModel.h"

class MobileView: public View {
public:
	MobileView(std::string type);
	virtual ~MobileView();
	void setModel(MobileModel *model);
	MotionDirection getMotionDirection();
	void render(Renderer* renderer );

protected:
	SDL_Point getOrigin();

private:
	MobileModel *model;
	SDL_Point lastOrigin;
	MotionDirection lastDirection;
};

#endif /* MOBILEVIEW_H_ */
