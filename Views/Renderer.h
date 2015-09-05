/*
 * Renderer.h
 *
 *  Created on: Aug 30, 2015
 *      Author: kaoru
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "View.h"
#include "Drawable.h"
#include <list>
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
	bool canDraw();
	void addView(View* view);

private:
	bool successfullInit;
	SDL_Window* window;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* marioTexture;
	Drawable* marioDrawable;
	list<View*> views;

	bool initSDL();
	bool loadMedia();
	bool loadTextureFromFile(std::string path, std::string name);
};

#endif /* RENDERER_H_ */
