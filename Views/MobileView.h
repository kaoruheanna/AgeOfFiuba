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
	void render(Renderer* renderer);
	bool hasModelWithId(int id);

protected:
	SDL_Point getOrigin();

private:
	Drawable* auxDrawable;
	bool isActive;
	MobileModel *model;
	SDL_Point lastOrigin;

	MotionDirection getMotionDirection(SDL_Point origen,SDL_Point destino, MotionDirection oldDirection);
	MotionDirection getMovingDirection();
	MotionDirection getInteractingDirection();
};

#endif /* MOBILEVIEW_H_ */
