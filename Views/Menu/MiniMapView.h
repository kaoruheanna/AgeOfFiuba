/*
 * MiniMapView.h
 *
 *  Created on: Oct 6, 2015
 *      Author: kaoru
 */

#ifndef MINIMAPVIEW_H_
#define MINIMAPVIEW_H_

#include <SDL2/SDL.h>

class View;
class Renderer;

class MiniMapView {
public:
	MiniMapView(View* view,SDL_Rect superviewRect);
	virtual ~MiniMapView();
	void render(Renderer* renderer);

private:
	View *view;
	SDL_Rect superviewRect;
};

#endif /* MINIMAPVIEW_H_ */
