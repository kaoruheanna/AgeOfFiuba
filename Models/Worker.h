/*
 * Worker.h
 *
 *  Created on: Oct 29, 2015
 *      Author: dario
 */

#ifndef MODELS_WORKER_H_
#define MODELS_WORKER_H_

#include "MobileModel.h"

class Worker: public MobileModel {
public:
	Worker();
	virtual ~Worker();

	virtual void interact(Entity* entity);

	virtual int getPoderCosecha();
};

#endif /* MODELS_WORKER_H_ */
