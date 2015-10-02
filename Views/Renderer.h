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
#include "ScreenMenu.h"

using namespace std;

class View;

class Renderer {
public:
	SDL_Point mainTilePosition;
	Renderer(int screenWidth, int screenHeight, list<TipoConfig> tipos);
	virtual ~Renderer();
	void close();
	void drawViews();

	// draw Drawable
	void draw(int mapPositionX, int mapPositionY, Drawable* drawable, bool iso);

	// draw shape
	void draw(SDL_Rect rect, SDL_Color color);

	SDL_Point mapToWindowPoint(SDL_Point mapPoint);
	SDL_Point windowToMapPoint(SDL_Point windowPoint);
	SDL_Point proyectedPoint(SDL_Point mapPoint, SDL_Point plano);
	bool canDraw();
	void addView(View* view);

private:
	bool successfullInit;
	SDL_Window* window;
	SDL_Renderer* sdlRenderer;
	list<View*> views;
	list< pair<SDL_Point,Drawable*> > drawablesToPaint;
	std::map<std::string, Drawable*> drawablesByInstanceName;
	Drawable* missingImageDrawable;
	ScreenMenu* screenMenu;

	int screenWidth;
	int screenHeight;

	bool initSDL();
	bool loadMedia(list<TipoConfig> tipos);
	bool isInsideWindow(SDL_Rect* rect);
	Drawable* getDrawableFromTipoConfig(TipoConfig tipo);
	void drawMenu();
};

#endif /* RENDERER_H_ */
