/*
 * MensajeroRed.h
 *
 *  Created on: Oct 18, 2015
 *      Author: nico
 */

#ifndef RED_MENSAJERORED_H_
#define RED_MENSAJERORED_H_

#include "../Controllers/Mensajero.h"

class MensajeroRed: public Mensajero {
public:
	MensajeroRed(int socket);
	virtual ~MensajeroRed();
	void setMensajero(Mensajero* escucha);
	void esperaMensaje();
	// Metodos Servidor -> Cliente
	virtual void errorDeLogueo();
	virtual void configEscenario(const string path);
	virtual void apareceRecurso(Resource* recurso);
	virtual void desapareceRecurso(Resource* recurso);
	virtual void actualizaPersonaje(MobileModel* entity);
	// Metodos Cliente -> Servidor
	virtual void loguearse(char* nombre);
	virtual void moverProtagonista(MobileModel* model);
private:
	int socket;
	Mensajero* escucha;
};

#endif /* RED_MENSAJERORED_H_ */
