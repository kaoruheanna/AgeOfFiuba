/*
 * TextLabel.h
 *
 *  Created on: Oct 4, 2015
 *      Author: kaoru
 */

#ifndef TEXTLABEL_H_
#define TEXTLABEL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;

class Renderer;

class TextLabel {
public:
	TextLabel(int x, int y);
	virtual ~TextLabel();
	void render(Renderer* renderer);

private:
	SDL_Point origin;
	int width;
	int height;
	SDL_Texture *texture;
	string message;

	void loadTexture(SDL_Renderer *sdlRenderer,TTF_Font *font);
};

#endif /* TEXTLABEL_H_ */
