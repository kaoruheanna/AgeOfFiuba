/*
 * PairIntSerializable.h
 *
 *  Created on: Nov 3, 2015
 *      Author: dario
 */

#ifndef RED_SERIALIZABLETEST_PAIRINTSERIALIZABLE_H_
#define RED_SERIALIZABLETEST_PAIRINTSERIALIZABLE_H_

#include "Serializable.h"

class PairIntSerializable: public Serializable  {
public:
	PairIntSerializable(int first, int second);
	PairIntSerializable();
	virtual ~PairIntSerializable();

	int first;
	int second;

	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
};

#endif /* RED_SERIALIZABLETEST_PAIRINTSERIALIZABLE_H_ */
