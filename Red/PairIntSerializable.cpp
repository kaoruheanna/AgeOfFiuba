/*
 * PairIntSerializable.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: dario
 */

#include "PairIntSerializable.h"
#include <memory.h>

PairIntSerializable::PairIntSerializable(int first, int second) {
	this->first = first;
	this->second = second;
}

PairIntSerializable::PairIntSerializable() {
	this->first = -1;
	this->second = -1;
}

PairIntSerializable::~PairIntSerializable() {}

// Serializable methods
int PairIntSerializable::getTotalBlockCount() {
	return 2;
}
int PairIntSerializable::getBlockSizeFromIndex(int currentIndex) {
	return sizeof(int);
}
void PairIntSerializable::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex == 0) {
		memcpy(buffer, &this->first, sizeof(int));
	} else {
		memcpy(buffer, &this->second, sizeof(int));
	}
}
void PairIntSerializable::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0) {
		memcpy(&this->first, blockData, sizeof(int));
	} else {
		memcpy(&this->second, blockData, sizeof(int));
	}
}
