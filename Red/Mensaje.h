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
	VACIO, /* Para inicializar los mensajes */
	LOGIN, /* Cliente => Servidor: Pide login con nombre */
	ERROR_NOMBRE_TOMADO, /* Servidor => Cliente: El usuario existe y esta conectado */
	ESCENARIO, /* Servidor => Cliente: Se logueo con el usuario y le esta mandando la info inicial */
	MOVER_PERSONAJE, /* Cliente => Servidor: Mover su personaje a la posicion */
	APARECE_PERSONAJE, /* Servidor => Cliente: Indica la posicion y estado de un personaje */
	APARECE_RECURSO, /* Servidor => Cliente: Indica que hay un recurso y en que posicion */
	ACTUALIZA_ENTIDAD, /* Servidor => Cliente: Indica que hay una actualizacion en una entity */
	DESAPARECE_RECURSO, /* Servidor => Cliente: Indica que se consumio un recurso */
	PING, /* Para probar si el servidor / cliente esta conectado */
	ERROR_MAXIMOS_EQUIPOS, /* Servidor => Cliente: No se puede loguear porque todos los equipos estan tomados */
	CAMBIO_USUARIO, /* Servidor => Cliente: Indica cuando un valor de un usuario cambio o cuando aparece un nuevo usuario */
	CONSTRUIR, /* Cliente => Servidor: Construir un edificio */
	COMENZO_PARTIDA /* Servidor => Cliente: Indica cuando la partida se puede empezar a jugar */
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
