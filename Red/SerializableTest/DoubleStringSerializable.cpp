/*
 * DoubleStringSerializable.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: nico
 */

#include "DoubleStringSerializable.h"

#include <string.h>
#include <stdlib.h>

DoubleStringSerializable::DoubleStringSerializable() {
	this->firstString = NULL;
	this->secondString = NULL;
}

DoubleStringSerializable::DoubleStringSerializable(const char* stringOne, const char* stringTwo) {
	this->firstString = (char*) malloc(strlen(stringOne) + 1);
	strcpy(this->firstString, stringOne);
	this->secondString = (char*) malloc(strlen(stringTwo) + 1);
	strcpy(this->secondString, stringTwo);
}

DoubleStringSerializable::~DoubleStringSerializable() {
	if(this->firstString != NULL){
		free(this->firstString);
		this->firstString = NULL;
	}
	if(this->secondString != NULL){
		free(this->secondString);
		this->secondString = NULL;
	}
}

char* DoubleStringSerializable::deserializeString(void* blockData) {
	char* toDeserialize = (char*) blockData;
	char* string = (char*) malloc(this->serializeStringSize(toDeserialize));
	this->serializeString(toDeserialize, (void*) string);
	return string;
}

void DoubleStringSerializable::serializeString(char* string, void* buffer) {
	strcpy((char*) buffer, string);
}

int DoubleStringSerializable::serializeStringSize(char* string) {
	return strlen(string) + 1;
}

// Complex serialization / deserialization

int DoubleStringSerializable::getTotalBlockCount() {
	return 2;
}

int DoubleStringSerializable::getBlockSizeFromIndex(int currentIndex) {
	if(currentIndex == 0){
		return this->serializeStringSize(this->firstString);
	} else {
		return this->serializeStringSize(this->secondString);
	}
}

void DoubleStringSerializable::getBlockFromIndex(int currentIndex, void* buffer) {
	if(currentIndex == 0){
		this->serializeString(this->firstString, buffer);
	} else {
		this->serializeString(this->secondString, buffer);
	}
}

void DoubleStringSerializable::deserialize(int totalBlockCount, int currentBlock, void* blockData) {
	if(currentBlock == 0){
		if(this->firstString != NULL){
			free(this->firstString);
			this->firstString = NULL;
		}
		this->firstString = this->deserializeString(blockData);
	} else {
		if(this->secondString != NULL){
			free(this->secondString);
			this->secondString = NULL;
		}
		this->secondString = this->deserializeString(blockData);
	}
}
