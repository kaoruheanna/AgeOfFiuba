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
using namespace std;

class Renderer {
public:
	Renderer();
	virtual ~Renderer();
	void close();
	void draw();
	bool canDraw();

private:
	SDL_Surface* gScreenSurface;
	SDL_Window* gWindow;
	SDL_Surface* gPNGSurface;
	bool successfullInit;

	bool initSDL();
	bool loadMedia();
	SDL_Surface* loadSurface(string path);
};

#endif /* RENDERER_H_ */
