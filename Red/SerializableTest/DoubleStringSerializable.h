/*
 * DoubleStringSerializable.h
 *
 *  Created on: Oct 14, 2015
 *      Author: nico
 */

#ifndef RED_SERIALIZABLETEST_DOUBLESTRINGSERIALIZABLE_H_
#define RED_SERIALIZABLETEST_DOUBLESTRINGSERIALIZABLE_H_

#include "StringSerializable.h"

class DoubleStringSerializable: public Serializable {
public:
	char* firstString;
	char* secondString;

	DoubleStringSerializable();
	DoubleStringSerializable(const char* stringOne, const char* stringTwo);
	virtual ~DoubleStringSerializable();
	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
private:
	char* deserializeString(void* blockData);
	void serializeString(char* string, void* buffer);
	int serializeStringSize(char* string);
};

#endif /* RED_SERIALIZABLETEST_DOUBLESTRINGSERIALIZABLE_H_ */
