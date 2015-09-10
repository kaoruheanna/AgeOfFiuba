/*
 * Renderer.h
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#define ISOMETRIC_DRAW false

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "View.h"
#include "Drawable.h"
#include <list>
#include <map>
using namespace std;

class View;

class Renderer {
public:
	SDL_Point mainTilePosition;
	Renderer();
	virtual ~Renderer();
	void close();
	void drawViews();
	void draw(int mapPositionX, int mapPositionY, Drawable* drawable);
	SDL_Point mapToWindowPoint(SDL_Point mapPoint);
	SDL_Point windowToMapPoint(SDL_Point windowPoint);
	bool canDraw();
	void addView(View* view);

private:
	bool successfullInit;
	SDL_Window* window;
	SDL_Renderer* sdlRenderer;
	list<View*> views;
	std::map<std::string, Drawable*> drawablesByInstanceName;
	Drawable* missingImageDrawable;

	bool initSDL();
	bool loadMedia();
};

#endif /* RENDERER_H_ */
