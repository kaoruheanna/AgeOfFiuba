/*
 * Serializable.h
 *
 *	Implementar esta clase en todos los modelos que vayamos a mandar por la red
 *	Una instancia de clase es representada por una cantidad variable de "bloques" (getTotalBlockCount)
 *	Cada "bloque" tiene un tamaño variable (getBlockSizeFromIndex)
 *	Se devuelve el "bloque" llenando el buffer con su binario (getBlockFromIndex)
 *	Cualquiera que herede esta clase tiene que implementar todos sus metodos
 *
 *  Created on: Oct 13, 2015
 *      Author: nico
 */

#ifndef RED_SERIALIZABLE_H_
#define RED_SERIALIZABLE_H_

class Serializable {
public:
	Serializable();
	virtual ~Serializable();
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
};

// Metodos para recibir y enviar serializables
int recibirSerializable(int sd, Serializable* serializable);
int enviarSerializable(int sd, Serializable* serializable);

#endif /* RED_SERIALIZABLE_H_ */
