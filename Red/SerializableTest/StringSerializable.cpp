/*
 * StringSerializable.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: nico
 */

#include "StringSerializable.h"

#include <string.h>
#include <stdlib.h>

StringSerializable::StringSerializable() {
	this->string = NULL;
}

StringSerializable::StringSerializable(const char* string) {
	this->string = (char*) malloc(strlen(string) + 1);
	strcpy(this->string, string);
}

StringSerializable::~StringSerializable() {
	if(this->string != NULL){
		free(this->string);
		this->string = NULL;
	}
}

int StringSerializable::getTotalBlockCount() {
	return 1;
}

int StringSerializable::getBlockSizeFromIndex(int currentIndex) {
	return strlen(this->string) + 1;
}

void StringSerializable::getBlockFromIndex(int currentIndex, void* buffer) {
	strcpy((char*) buffer, this->string);
}

void StringSerializable::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0){
		char* data = (char*) blockData;
		if(this->string != NULL){
			free(this->string);
			this->string = NULL;
		}
		this->string = (char*) malloc(strlen(data) + 1);
		strcpy(this->string, data);
	}
}

