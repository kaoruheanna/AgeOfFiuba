/*
 * GameConfiguration.h
 *
 *  Created on: 08/09/2015
 *      Author: die89
 */


/*
 * GameConfiguration.h
 *
 *  Created on: 08/09/2015
 *      Author: die89
 */

#ifndef GAMECONFIGURATION_H_
#define GAMECONFIGURATION_H_

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <string>
#include "PantallaConfig.h"
#include "ConfiguracionConfig.h"
#include "TipoConfig.h"


//const char* ARCHIVOPORDEFECTO = "configuracion.yaml";

class GameConfiguration {

public:
	GameConfiguration();
	GameConfiguration(const char* archivoAParsear);
	~GameConfiguration();
	int getPantallaAlto();
	int getPantallaAncho();
	int getVelocidadPersonaje();
	int getMargenScroll();
	list<TipoConfig> getTipos();

private:
	void parseYAML(const char* archivoAParsear);
	void loadDefaultConfiguration();
	bool loadFile(const char* archivoAParsear);
	void auxiliar();
	const char* defaultFile;
	YAML::Node nodoRaiz;
	PantallaConfig pantalla;
	ConfiguracionConfig configuracion;
	list<TipoConfig> tipos;
};

#endif /* GAMECONFIGURATION_H_ */












































