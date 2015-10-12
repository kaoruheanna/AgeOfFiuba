/*
 * Cliente.h
 *
 *  Created on: Oct 3, 2015
 *      Author: nico
 */

#ifndef RED_CLIENTE_H_
#define RED_CLIENTE_H_

class Cliente {
public:
	Cliente();
	virtual ~Cliente();
	void empezar(char* serverIp, int serverPort);
private:
	void mandarTexto();
	void cerrarConeccion();
};

#endif /* RED_CLIENTE_H_ */
