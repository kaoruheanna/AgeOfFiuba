/*
 * EscenarioConfig.h
 *
 *  Created on: 16/09/2015
 *      Author: die89
 */

#ifndef ESCENARIOCONFIG_H_
#define ESCENARIOCONFIG_H_

#include <yaml-cpp/yaml.h>


class EscenarioConfig {
public:
	EscenarioConfig();
	EscenarioConfig(YAML::Node nodo);
	~EscenarioConfig();

private:
	YAML::Node nodoEscenario;
	void validarContenido();
	void cargarEscenarioPorDefecto();
};

#endif /* ESCENARIOCONFIG_H_ */
