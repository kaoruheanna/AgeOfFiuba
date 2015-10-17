/*
 * Archivo.h
 *
 *  Created on: Oct 17, 2015
 *      Author: nico
 */

#ifndef RED_ARCHIVO_H_
#define RED_ARCHIVO_H_

#include "Serializable.h"

class Archivo: public Serializable {
public:
	Archivo(const char* archivo);
	virtual ~Archivo();
	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
private:
	const char* path;
	char* contenido;
	void cargarArchivo();
};

#endif /* RED_ARCHIVO_H_ */
