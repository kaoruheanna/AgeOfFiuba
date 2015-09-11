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
	virtual SDL_Rect getRectToDraw(int windowMainTilePositionX, int windowMainTilePositionY);
	SDL_Texture* getTexture();
	bool loadTextureFromFile(std::string path,SDL_Renderer* sdlRenderer);
	virtual SDL_Rect* getClipRect();
	virtual void animate();
protected:
	bool loadTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);
	virtual void onTextureChange();

	SDL_Texture *texture;
	SDL_Point mainTilePosition;
	int height;
	int width;
private:
	/*
	 * Hay que aclarar que es cada cosa
	 */
	int baseTileWidth;
	int baseTileHeight;


};

#endif /* VIEWS_DRAWABLE_H_ */
