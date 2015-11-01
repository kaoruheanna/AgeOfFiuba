/*
 * EntidadConfig.h
 *
 *  Created on: Sep 17, 2015
 *      Author: nico
 */

#ifndef CONFIGURATION_ENTIDADCONFIG_H_
#define CONFIGURATION_ENTIDADCONFIG_H_

#include <string>
#include <yaml-cpp/yaml.h>

using namespace std;

class EntidadConfig {
public:
	EntidadConfig();
	EntidadConfig(YAML::Node nodo);
	EntidadConfig(int x, int y, string tipo, string equipo); // TODO Borrar y parsearlo del YAML
	virtual ~EntidadConfig();

	int getX();
	int getY();
	string getTipo();
	string getEquipo();
private:
	int x;
	int y;
	string tipo;
	string equipo;

	static YAML::Node findNodeByName(YAML::Node nodo, string attributeName);
	static int getIntAttribute(YAML::Node nodo, string attributeName, int defaultValue);
	static string getStringAttribute(YAML::Node nodo, string attributeName, string defaultValue);
};

#endif /* CONFIGURATION_ENTIDADCONFIG_H_ */
