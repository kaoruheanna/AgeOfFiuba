/*
 * Servidor.h
 *
 *  Created on: Oct 3, 2015
 *      Author: nico
 */

#ifndef RED_SERVIDOR_H_
#define RED_SERVIDOR_H_

class Servidor {
public:
	Servidor();
	virtual ~Servidor();
	void empezar(int port);
private:
	bool existeUsuario(char* nombre);
	bool usuarioLogueado(char* nombre);
};

#endif /* RED_SERVIDOR_H_ */
