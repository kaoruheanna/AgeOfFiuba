/*
 * Worker.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: dario
 */

#include "Worker.h"

Worker::Worker() {
	// TODO Auto-generated constructor stub

}

Worker::~Worker() {
	// TODO Auto-generated destructor stub
}

void Worker::interact(Entity* entity) {
	entity->receiveInteraction(this);
}

int Worker::getPoderCosecha() {
	return 1;
}
