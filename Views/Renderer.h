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
#include "Texture.h"
#include "View.h"
#include <list>
using namespace std;

class Renderer {
public:
	Renderer();
	virtual ~Renderer();
	void close();
	void draw();
	bool canDraw();
	void addView(View* view);

private:
	bool successfullInit;
	SDL_Window* window;
	SDL_Renderer* sdlRenderer;
	Texture* marioTexture;
	list<View*> views;

	bool initSDL();
	bool loadMedia();
};

#endif /* RENDERER_H_ */
