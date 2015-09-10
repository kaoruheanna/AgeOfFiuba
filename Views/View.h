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

class Renderer;

class View {
public:
	View(std::string type);
	virtual ~View();
	virtual void render(Renderer* renderer);
	virtual void render(Renderer* renderer, bool iso);
	void setDrawable(Drawable *drawable);
	void setOrigin(int x, int y);
	std::string getType();

protected:
	virtual SDL_Point getOrigin();

	SDL_Point origin;
	Drawable *drawable;

private:
	std::string type;

};

#endif /* VIEW_H_ */
