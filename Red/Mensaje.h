/*
 * Mensaje.h
 *
 *  Created on: Oct 14, 2015
 *      Author: nico
 */

#ifndef RED_MENSAJE_H_
#define RED_MENSAJE_H_

#include "Serializable.h"

enum MensajeType {
	VACIO, /* Para inicializar los mensajes / Pingear (???) */
	LOGIN, /* Cliente => Servidor: Pide login con nombre */
	ERROR_NOMRE_TOMADO, /* Servidor => Cliente: El usuario existe y esta conectado */
	ESCENARIO, /* Servidor => Cliente: Se logueo con el usuario y le esta mandando la info inicial */
	MOVER_PERSONAJE, /* Cliente => Servidor: Mover su personaje a la posicion */
	APARECE_PERSONAJE, /* Servidor => Cliente: Indica la posicion y estado de un personaje */
	APARECE_RECURSO, /* Servidor => Cliente: Indica que hay un recurso y en que posicion */
	DESAPARECE_RECURSO /* Servidor => Cliente: Indica que se consumio un recurso */
};

class Mensaje: public Serializable {
public:
	Mensaje();
	Mensaje(MensajeType type, const char* sender);
	virtual ~Mensaje();
	// Mensaje methods
	MensajeType getType();
	char* getSender();
	char* toString(); // Para logueo
	// Serializable methods
	virtual int getTotalBlockCount();
	virtual int getBlockSizeFromIndex(int currentIndex);
	virtual void getBlockFromIndex(int currentIndex, void* buffer);
	virtual void deserialize(int totalBlockCount, int currentBlock, void* blockData);
private:
	MensajeType type;
	char* sender;
	char* logString;

	char* deserializeString(void* blockData);
	void serializeString(char* string, void* buffer);
	int serializeStringSize(char* string);
};

#endif /* RED_MENSAJE_H_ */
