/*
 * PartidaRey.h
 *
 *  Created on: Nov 18, 2015
 *      Author: nico
 */

#ifndef MODELS_TIPOPARTIDA_PARTIDAREY_H_
#define MODELS_TIPOPARTIDA_PARTIDAREY_H_

#include "TipoPartida.h"
#include "../../GlobalConstants.h"

class PartidaRey: public TipoPartida {
public:
	PartidaRey();
	virtual ~PartidaRey();

	virtual void calcularEstado(list<Entity*> entidades);
};

#endif /* MODELS_TIPOPARTIDA_PARTIDAREY_H_ */
