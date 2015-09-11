/*
 * Sprite.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: nico
 */

#include "Sprite.h"

Sprite::Sprite(int mainTilePositionX, int mainTilePositionY,int baseTileWidth, int baseTileHeight, int spriteWidth, int spriteHeight, int fps)
: Drawable(mainTilePositionX, mainTilePositionY, baseTileWidth, baseTileHeight){
	this->currentFrame = 0;
	this->currentAnimation = 0;
	this->fps = fps;
	this->height = spriteHeight;
	this->width = spriteWidth;
	this->animationCount = 0;
	this->framesPerAnimation = 0;
	this->clipRect = {
		0, 0,
		this->width,
		this->height
	};
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}

void Sprite::onTextureChange(){
	int w, h;
	SDL_QueryTexture(this->texture, NULL, NULL, &w, &h);
	this->animationCount = h / this->height;
	this->framesPerAnimation = w / this->width;
}

void Sprite::selectAnimation(int animation){
	if(animation > this->animationCount){
		this->currentAnimation = 0;
		printf("Selected unexisting animation!!");
	} else {
		this->currentAnimation = animation;
	}
	this->currentFrame = 0;
}

SDL_Rect* Sprite::getClipRect(){
	return &this->clipRect;
}

void Sprite::animate(){
	// Count frame
	this->currentFrame ++;
	if(this->currentFrame >= (this->framesPerAnimation * this->fps)){
		// If frame is over animation restart it
		this->currentFrame = 0;
	}
	// Move clip to animation frame
	int currentAnimationFrame = this->currentFrame / this->fps;
	this->clipRect.x = currentAnimationFrame * this->width;
	this->clipRect.y = currentAnimation * this->height;
}
