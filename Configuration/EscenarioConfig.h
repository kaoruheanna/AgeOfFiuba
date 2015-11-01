/*
 * EscenarioConfig.h
 *
 *  Created on: 16/09/2015
 *      Author: die89
 */

#ifndef ESCENARIOCONFIG_H_
#define ESCENARIOCONFIG_H_

#include <yaml-cpp/yaml.h>
#include <list>
#include <string>
#include "EntidadConfig.h"

using namespace std;

const int TAMANIO_DEFAULT = 100;

class EscenarioConfig {
public:
	EscenarioConfig(); // TODO Borrar cuando se alla implementado el de abajo
	EscenarioConfig(YAML::Node nodo);
	~EscenarioConfig();

	std::list<EntidadConfig> getEntidades();
	std::string getNombre();
	int getSizeX();
	int getSizeY();
	EntidadConfig getProtagonista();

private:
	YAML::Node nodoEscenario;
	void validarContenido();
	void cargarEscenarioPorDefecto();
	void validarTamanio();
	bool verificarNombre();
	bool validarINT(std::string);
	void parsearProtagonista();
	void parsearEntidades();

	int sizeX;
	int sizeY;
	std::string nombre;
	EntidadConfig protagonista;
	std::list<EntidadConfig> lista;
};

#endif /* ESCENARIOCONFIG_H_ */
