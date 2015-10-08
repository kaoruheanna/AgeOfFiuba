/*
 * MiniView.h
 *
 *  Created on: Oct 11, 2015
 *      Author: kaoru
 */

#ifndef MINIVIEW_H_
#define MINIVIEW_H_

#include <SDL2/SDL.h>
#include <string>

class Drawable;
class Renderer;

class MiniView {
public:
	MiniView(std::string type);
	virtual ~MiniView();
	void setDrawable(Drawable *drawable);
	void setOrigin(int x, int y);
	void render(Renderer* renderer);
	std::string getType();

protected:
	Drawable *drawable;

private:
	SDL_Point origin;
	std::string type;
};

#endif /* MINIVIEW_H_ */
