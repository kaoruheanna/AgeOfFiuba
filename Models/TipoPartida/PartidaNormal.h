/*
 * PartidaNormal.h
 *
 *  Created on: Nov 18, 2015
 *      Author: nico
 */

#ifndef MODELS_TIPOPARTIDA_PARTIDANORMAL_H_
#define MODELS_TIPOPARTIDA_PARTIDANORMAL_H_

#include "TipoPartida.h"
#include "../../GlobalConstants.h"

class PartidaNormal: public TipoPartida {
public:
	PartidaNormal();
	virtual ~PartidaNormal();

	virtual void calcularEstado(list<Entity*> entidades);
};

#endif /* MODELS_TIPOPARTIDA_PARTIDANORMAL_H_ */
