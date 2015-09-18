/*
 * EntidadConfig.h
 *
 *  Created on: Sep 17, 2015
 *      Author: nico
 */

#ifndef CONFIGURATION_ENTIDADCONFIG_H_
#define CONFIGURATION_ENTIDADCONFIG_H_

#include <string>

class EntidadConfig {
public:
	EntidadConfig(int x, int y, std::string tipo); // TODO Borrar y parsearlo del YAML
	virtual ~EntidadConfig();

	int getX();
	int getY();
	std::string getTipo();
private:
	int x;
	int y;
	std::string tipo;
};

#endif /* CONFIGURATION_ENTIDADCONFIG_H_ */
