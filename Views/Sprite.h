/*
 * Sprite.h
 *
 *  Created on: Sep 10, 2015
 *      Author: nico
 */

#ifndef VIEWS_SPRITE_H_
#define VIEWS_SPRITE_H_

#include <list>
#include "Drawable.h"
#include "MobileView.h"
using namespace std;

class Sprite: public Drawable {
public:
	Sprite(int mainTilePositionX, int mainTilePositionY,int baseTileWidth, int baseTileHeight, int spriteWidth, int spriteHeight, int fps);
	virtual ~Sprite();
	SDL_Rect* getClipRect();
	void selectAnimation(MotionDirection direction);
	void animate();

protected:
	void onTextureChange();

private:
	int currentFrame;
	MotionDirection currentAnimation;
	int fps;
	int animationCount;
	int framesPerAnimation;
	SDL_Rect clipRect;
};

#endif /* VIEWS_SPRITE_H_ */
