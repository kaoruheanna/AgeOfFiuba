/*
 * Drawable.h
 *
 *  Created on: Sep 5, 2015
 *      Author: nico
 */

#ifndef VIEWS_DRAWABLE_H_
#define VIEWS_DRAWABLE_H_

#include <SDL2/SDL.h>
#include "string"

class Drawable {
public:
	Drawable(int mainTilePositionX, int mainTilePositionY, int baseTileWidth, int baseTileHeight);
	virtual ~Drawable();
	SDL_Rect getRectToDraw(int windowMainTilePositionX, int windowMainTilePositionY);
	SDL_Texture* getTexture();
	bool loadTextureFromFile(std::string path,SDL_Renderer* sdlRenderer);

private:
	/*
	 * Hay que aclarar que es cada cosa
	 */
	SDL_Point mainTilePosition;
	int baseTileWidth;
	int baseTileHeight;
	SDL_Texture *texture;


};

#endif /* VIEWS_DRAWABLE_H_ */
