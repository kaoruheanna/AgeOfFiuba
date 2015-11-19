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

enum MotionDirection {
	SOUTH = 0,
	SOUTH_WEST,
	WEST,
	NORTH_WEST,
	NORTH,
	SOUTH_EAST,
	EAST,
	NORTH_EAST,
};

struct AnimationStatus {
	MotionDirection direction;
	int animationIndex;
	bool isMoving;
	bool isInteracting;
};


class Drawable {
public:
	Drawable(int mainTilePositionX, int mainTilePositionY);
	virtual ~Drawable();
	virtual SDL_Rect getRectToDraw(int windowMainTilePositionX, int windowMainTilePositionY);
	SDL_Texture* getTexture();
	bool loadTextureFromFile(std::string path,SDL_Renderer* sdlRenderer);
	virtual SDL_Rect* getClipRect();
	virtual void animate(AnimationStatus status);
	virtual AnimationStatus getAnimation(MotionDirection currentDirection, bool currentlyMoving, bool currentlyInteracting, AnimationStatus lastStatus);
	void free();
	int getHeight();

protected:
	bool loadTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);
	virtual void onTextureChange();

	SDL_Texture *texture;
	SDL_Point mainTilePosition;
	int height;
	int width;

};

#endif /* VIEWS_DRAWABLE_H_ */
