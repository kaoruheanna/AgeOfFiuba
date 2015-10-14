/*
 * StringSerializable.h
 *
 *  Created on: Oct 14, 2015
 *      Author: nico
 */

#ifndef RED_SERIALIZABLETEST_STRINGSERIALIZABLE_H_
#define RED_SERIALIZABLETEST_STRINGSERIALIZABLE_H_

#include "../Serializable.h"

class StringSerializable: public Serializable {
public:
	char* string;

	StringSerializable();
	StringSerializable(const char* string);
	virtual ~StringSerializable();
	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
};

#endif /* RED_SERIALIZABLETEST_STRINGSERIALIZABLE_H_ */
