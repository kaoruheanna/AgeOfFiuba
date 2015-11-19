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
	virtual void setDrawableDeshabilitado(Drawable *drawable);
	Drawable* getDrawable();
	MotionDirection getMotionDirection();
	void render(Renderer* renderer);

protected:
	SDL_Point getOrigin();

private:
	Drawable* auxDrawable;
	bool isActive;
	MobileModel *model;
	SDL_Point lastOrigin;
};

#endif /* MOBILEVIEW_H_ */
