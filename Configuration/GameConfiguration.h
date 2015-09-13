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

//const char* ARCHIVOPORDEFECTO = "configuracion.yaml";

class GameConfiguration {

public:
	GameConfiguration();
	GameConfiguration(const char* archivoAParsear);
	~GameConfiguration();
	int getPantallaAlto();
	int getPantallaAncho();


private:
	void parseYAML(const char* archivoAParsear);
	void loadDefaultConfiguration();
	bool loadFile(const char* archivoAParsear);
	void auxiliar();
	bool verificarPantalla(YAML::Node& nodo);
	const char* defaultFile;
	YAML::Node nodoRaiz;
};

#endif /* GAMECONFIGURATION_H_ */












































