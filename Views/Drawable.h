/*
 * Drawable.h
 *
 *  Created on: Sep 5, 2015
 *      Author: nico
 */

#ifndef VIEWS_DRAWABLE_H_
#define VIEWS_DRAWABLE_H_

#include <SDL2/SDL.h>

class Drawable {
public:
	Drawable(int mainTilePositionX, int mainTilePositionY, int baseTileWidth, int baseTileHeight, SDL_Texture* texture);
	virtual ~Drawable();
	SDL_Rect getRectToDraw(int windowMainTilePositionX, int windowMainTilePositionY);
	SDL_Texture* getTexture();
private:
	SDL_Point mainTilePosition;
	int baseTileWidth;
	int baseTileHeight;
	SDL_Texture *texture;
};

#endif /* VIEWS_DRAWABLE_H_ */
