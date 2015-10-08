/*
 * Mapeable.h
 *
 *  Created on: Oct 6, 2015
 *      Author: dario
 */

#ifndef MODELS_MAPEABLE_H_
#define MODELS_MAPEABLE_H_

class Mapeable {
public:
	Mapeable(){};
	virtual ~Mapeable(){};
	virtual bool OcupaEspacio() {return true;};
	virtual bool PuedeAtravesarse() {return false;};
};

#endif /* MODELS_MAPEABLE_H_ */
