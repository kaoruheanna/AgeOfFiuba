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
	Mapeable(){Cosechable = false;};
	virtual ~Mapeable(){};

	bool Cosechable;
};

#endif /* MODELS_MAPEABLE_H_ */
