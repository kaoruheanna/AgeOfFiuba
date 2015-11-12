/*
 * Cliente.h
 *
 *  Created on: Oct 3, 2015
 *      Author: nico
 */

#ifndef RED_CLIENTE_H_
#define RED_CLIENTE_H_

#include "../Utils/ServerConectionView.h"

class Cliente{
public:
	Cliente();
	virtual ~Cliente();
	void empezar(char* serverIp, int serverPort, string username);
	void mostrarLogin();
private:
	ServerConectionView* loginView;
};

#endif /* RED_CLIENTE_H_ */
