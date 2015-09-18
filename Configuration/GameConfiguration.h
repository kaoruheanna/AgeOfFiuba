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
#include "EscenarioConfig.h"


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
	EscenarioConfig getEscenario();

private:
	void parseYAML(const char* archivoAParsear);
	void loadFile(const char* archivoAParsear);
	void auxiliar();
	const char* defaultFile;
	YAML::Node nodoRaiz;
	PantallaConfig pantalla;
	ConfiguracionConfig configuracion;
	list<TipoConfig> tipos;
	EscenarioConfig escenario;
};

#endif /* GAMECONFIGURATION_H_ */












































