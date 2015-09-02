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

	Texture();
	~Texture();
	bool loadFromFile( std::string path,SDL_Renderer* sdlRenderer );
	void free();
	void render( int x, int y,SDL_Renderer* sdlRenderer ); //ver si esta sigue asi o lo renderiza otro
	int getWidth();
	int getHeight();

private:
	SDL_Texture* sdlTexture;
	int width;
	int height;

};

#endif /* TEXTURE_H_ */
