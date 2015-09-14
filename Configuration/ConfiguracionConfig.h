/*
 * ConfiguracionConfig.h
 *
 *  Created on: 13/09/2015
 *      Author: die89
 */

#ifndef CONFIGURACIONCONFIG_H_
#define CONFIGURACIONCONFIG_H_

#include <yaml-cpp/yaml.h>

const int VELOCIDAD_PERSONAJE_DEFAULT = 5;
const int MARGEN_SCROLL_DEFAULT = 15;

class ConfiguracionConfig {
public:
	ConfiguracionConfig();
	ConfiguracionConfig(YAML::Node nodo);
	~ConfiguracionConfig();
	int getVelocidad();
	int getScroll();

private:
	YAML::Node nodoConfiguracion;
	void verificarDatos();
	bool verificarVelocidad();
	bool verificarScroll();
};

#endif /* CONFIGURACIONCONFIG_H_ */
