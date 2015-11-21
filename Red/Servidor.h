/*
 * Servidor.h
 *
 *  Created on: Oct 3, 2015
 *      Author: nico
 */

#ifndef RED_SERVIDOR_H_
#define RED_SERVIDOR_H_

#include "../Controllers/ServerGameController.h"

class Servidor {
public:
	ServerGameController* modelos;

	Servidor();
	virtual ~Servidor();
	void empezar(int port, const char* configPath);
};

#endif /* RED_SERVIDOR_H_ */
