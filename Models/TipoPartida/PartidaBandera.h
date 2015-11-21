/*
 * PartidaBandera.h
 *
 *  Created on: Nov 21, 2015
 *      Author: nico
 */

#ifndef MODELS_TIPOPARTIDA_PARTIDABANDERA_H_
#define MODELS_TIPOPARTIDA_PARTIDABANDERA_H_

#include "TipoPartida.h"
#include "../../GlobalConstants.h"

class PartidaBandera: public TipoPartida {
public:
	PartidaBandera();
	virtual ~PartidaBandera();

	virtual void calcularEstado(list<Entity*> entidades);
};

#endif /* MODELS_TIPOPARTIDA_PARTIDABANDERA_H_ */
