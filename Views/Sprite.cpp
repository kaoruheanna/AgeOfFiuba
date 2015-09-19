/*
 * Sprite.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: nico
 */

#include "Sprite.h"
#include "../GlobalConstants.h"
#include "../Utils/Log.h"

#define STANDING_SPRITE_INDEX 	5
#define START_MOVING_INDEX 		0

const std::string TAG = "Sprite";

Sprite::Sprite(int mainTilePositionX, int mainTilePositionY, int spriteWidth, int spriteHeight, float fps, float delay)
: Drawable(mainTilePositionX, mainTilePositionY){

	this->fps = fps;
	this->delay = delay;
	this->height = spriteHeight;
	this->width = spriteWidth;
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

	int framesPerAnimation = w / this->width;

	int repeatTimes = DELAY_MILISEC * this->fps / framesPerAnimation;
	Log().Get(TAG,logINFO) << "repeatTimes: "<< repeatTimes;

	// guarda las coordenadas en x de cada frame de animacion
	for (int i = START_MOVING_INDEX ; i < framesPerAnimation ; i++){
		if (i != STANDING_SPRITE_INDEX){
			for (int j = 0; j < repeatTimes; j++){
				this->frameIndexes.push_back(i);
				Log().Get(TAG,logINFO) << "agrego el indice de la animacion: "<< i;
			}
		}
	}
}

AnimationStatus Sprite::getAnimation(MotionDirection currentDirection, bool currentlyMoving, AnimationStatus lastStatus) {
	AnimationStatus newStatus;
	newStatus.direction = currentDirection;
	newStatus.isMoving = currentlyMoving;

	// si no se esta moviendo no importa el animationIndex
	if (!currentlyMoving){
		return newStatus;
	}

	if (lastStatus.direction != currentDirection){
		newStatus.animationIndex = START_MOVING_INDEX;
		return newStatus;
	}

	int index = lastStatus.animationIndex;
	index++;
	if (index >= (int)this->frameIndexes.size()){
		index = 0;
	}
	newStatus.animationIndex = index;
	return newStatus;
}

void Sprite::animate(AnimationStatus status){
	int frameIndex = (status.isMoving) ? this->frameIndexes[status.animationIndex] : STANDING_SPRITE_INDEX ;
//	Log().Get(TAG,logINFO) << "indice de la animacion: "<< frameIndex;
	this->clipRect.x = frameIndex * this->width;
	this->clipRect.y = status.direction * this->height;
}

SDL_Rect* Sprite::getClipRect(){
	return &this->clipRect;
}
