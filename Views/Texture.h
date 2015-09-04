/*
 * Texture.h
 *
 *  Created on: Sep 2, 2015
 *      Author: kaoru
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

class Texture {
public:
	Texture(std::string path,SDL_Renderer* sdlRenderer);
	~Texture();

	void free();
	void render(SDL_Point origin,SDL_Renderer* sdlRenderer );

private:
	SDL_Texture* sdlTexture;
	int width;
	int height;

	bool loadFromFile(std::string path,SDL_Renderer* sdlRenderer);
	int getWidth();
	int getHeight();
};

#endif /* TEXTURE_H_ */
