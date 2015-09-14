/*
 * Renderer.h
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#define ISOMETRIC_DRAW true
#define ISOMETRIC_ROTATION_FACTOR 0.707

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "View.h"
#include "Drawable.h"
#include <list>
#include <map>
#include "../Configuration/TipoConfig.h"
using namespace std;

class View;

class Renderer {
public:
	SDL_Point mainTilePosition;
	Renderer(int screenWidth, int screenHeight, list<TipoConfig> tipos);
	virtual ~Renderer();
	void close();
	void drawViews();
	void draw(int mapPositionX, int mapPositionY, Drawable* drawable, bool iso);
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

	int screenWidth;
	int screenHeight;

	bool initSDL();
	bool loadMedia(list<TipoConfig> tipos);
};

#endif /* RENDERER_H_ */
