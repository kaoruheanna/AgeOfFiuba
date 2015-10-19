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


class GameConfiguration {

public:
	GameConfiguration();
	GameConfiguration(const string archivoAParsear);
	~GameConfiguration();
	int getPantallaAlto();
	int getPantallaAncho();
	int getVelocidadPersonaje();
	int getMargenScroll();
	list<TipoConfig> getTipos();
	EscenarioConfig getEscenario();
	const std::string getPath();

	std::string getNombreEscenario();
	int getTamanioX();
	int getTamanioY();
	PantallaConfig pantalla;
	ConfiguracionConfig configuracion;

private:
	void parseYAML(const string archivoAParsear);
	void loadFile(const string archivoAParsear);
	void auxiliar();
	YAML::Node nodoRaiz;
	std::string path;

	list<TipoConfig> tipos;
	EscenarioConfig escenario;
};

#endif /* GAMECONFIGURATION_H_ */












































