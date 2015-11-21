/*
 * View.h
 *
 *  Created on: Sep 3, 2015
 *      Author: kaoru
 */

#ifndef VIEW_H_
#define VIEW_H_

#include "Renderer.h"
#include "Drawable.h"
#include "../GlobalConstants.h"

class Renderer;

class View {
public:
	View(std::string type);
	virtual ~View();
	virtual void render(Renderer* renderer);
	void setDrawable(Drawable *drawable);
	void setInteractingDrawable(Drawable *drawable);
	Drawable* getDrawable();
	virtual void setDrawableDeshabilitado(Drawable *drawable);
	void setOrigin(int x, int y);
	virtual SDL_Point getOrigin();
	std::string getType();
	virtual bool hasModelWithId(int id);

protected:
	SDL_Point origin;
	Drawable *drawable;
	Drawable *interactingDrawable;
	AnimationStatus animationStatus;

private:
	std::string type;

};

#endif /* VIEW_H_ */
