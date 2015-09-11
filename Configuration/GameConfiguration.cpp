/*
 * GameConfiguration.cpp
 *
 *  Created on: 08/09/2015
 *      Author: die89
 */




#include "GameConfiguration.h"



GameConfiguration::GameConfiguration(const char* archivoAParsear){
	this->defaultFile = "configuracion.yaml";//ARCHIVO_DEFAULT;
	this->parseYAML(archivoAParsear);
}

GameConfiguration::GameConfiguration(){
	this->defaultFile = "configuracion.yaml";
	this->loadDefaultConfiguration();
}

GameConfiguration::~GameConfiguration(){

}

//ESTA FUNCION VA A COMPROBAR QUE EL ARCHIVO RESPETE LAS ESPECIFICACIONES DE YAML, NO VERIFICA LA VALIDACION DE VALORES
bool GameConfiguration::loadFile(const char* archivoAParsear){
	try{
	this->nodoRaiz = YAML::LoadFile(archivoAParsear);
	}
	catch ( YAML::BadFile& archivoCorrupto){
		this->nodoRaiz = NULL;
		return false;
	}
	return true;
}



void GameConfiguration::loadDefaultConfiguration(){
	this->nodoRaiz = YAML::LoadFile(this->defaultFile);
}

void GameConfiguration::parseYAML(const char* archivoAParsear){
	//VERIFICA SI ARCHIVO ESTA CORRUPTO O TIENE FORMATO VALIDO DE YAML, EN ESE CASO PARSEA DIRECTAMENTE EL ARCHIVO POR DEFECTO
	if ( !this->loadFile(archivoAParsear) ){
		//DEBE LOGUEAR QUE EL ARCHIVO PASADO POR PARAMETRO ESTA CORRUPTO Y POR ENDE SE CARGARON LOS VALORES DEL ARCHIVO DEFAULT
		this->loadDefaultConfiguration();
		//SI ES EL DEFAULT NO HAY QUE HACER EL TRABAJO QUE VIENE DESPUES
		return;
	}
	YAML::Node nodoPantalla = this->nodoRaiz["pantalla"];
	YAML::Node nodoTipos = this->nodoRaiz["tipos"];
	YAML::Node nodoConfiguracion = this->nodoRaiz["configuracion"];
	YAML::Node nodoEscenario = this->nodoRaiz["escenario"];
	//REALIZAR VERIFICACIONES DEL FORMATO DE CADA NODO

}






























/*#include "GameConfiguration.h"
//VALIDAR ARCHIVO
//VALORES INVALIDOS
//ANTE CUALQUIER VALOR INVALIDO, CARGAR ARCHIVO DEFAULT;

GameConfiguration::GameConfiguration() {
	this->defaultFile = "configuracion.yaml"; //DEFAULT_FILE;
	this->altoPantalla = ALTO_PANTALLA_DEFAULT;
	this->anchoPantalla = ANCHO_PANTALLA_DEFAULT;
	this->margen_scroll = MARGEN_SCROLL_DEFAULT;
	this->vel_personaje = VEL_PERSONAJE_DEFAULT;
	this->tipoProtagonista = PROTAGONISTA_DEFAULT;

}

GameConfiguration::~GameConfiguration() {
	// TODO Auto-generated destructor stub
}

void GameConfiguration::ParseYAML(const char* archivoAParsear){
//ESTE IF DEBE VERIFICAR SI EL ARCHIVO ES CORRECTO O ESTA CORRUPTO
if( 1 == 1){
	//FALTA QUE SE FIJE SI ESTA BIEN O MAL FORMADO EL ARCHIVO PARA VER SI CARGA EL DEFAULT O EL DE PARAMETRO
	//Esto deberia cargar el archivo que se escribe como argumento en la terminal, por ahora carga el DEFAULT
	YAML::Node  nodos = YAML::LoadFile(this->defaultFile);
	 if (nodos){
			 // Valida que las dimensiones de la pantalla sean por lo menos de 640 x 480
		 	 if (nodos["pantalla"]){
				   YAML::Node nodoPantalla = nodos["pantalla"];
				   for (std::size_t i=0;i<nodoPantalla.size();i++) {
				     if ((nodoPantalla[i]["alto"].IsScalar()) && (nodoPantalla[i]["alto"].as<int>() >= MIN_ALTO_PANTALLA)) {
				    	 altoPantalla = nodoPantalla[i]["alto"].as<int>();
				     }
				     if ((nodoPantalla[i]["ancho"].IsScalar()) && (nodoPantalla[i]["ancho"].as<int>() >= MIN_ANCHO_PANTALLA)) {
				       	 anchoPantalla = nodoPantalla[i]["ancho"].as<int>();
				     }

				   }
		     }
			 else {
				   //Se debe loguear que se mantuvieron los valores por DEFAULT ya que no existe nodo Pantalla
				 	 this->LoadDefaultConfiguration();
			 }
			 if (nodos["configuracion"]){
				 YAML::Node nodoConfig = nodos["configuracion"];
				 for (std::size_t i=0;i<nodoConfig.size();i++) {
				 	if ((nodoConfig[i]["vel_personaje"].IsScalar()) && (nodoConfig[i]["vel_personaje"].as<int>() >= MIN_VEL_PERSONAJE)) {
				 			vel_personaje = nodoConfig[i]["vel_personaje"].as<int>();
				     }
			        if ((nodoConfig[i]["margen_scroll"].IsScalar()) && (nodoConfig[i]["margen_scroll"].as<int>() >= 0)) {
					       	margen_scroll = nodoConfig[i]["margen_scroll"].as<int>();
				     }
 				   }
			 }
			 if (nodos["tipos"]){
				 YAML::Node nodoTipos = nodos["tipos"];
				 for (std::size_t i=0;i<nodoTipos.size();i++) {

				 }

			 }
			 if (nodos["escenario"]){
				 YAML::Node nodoEscenario = nodos["escenario"];
				 for (std::size_t i=0;i<nodoEscenario.size();i++){
					 if (nodoEscenario[i]["protagonista"]){
						 YAML::Node nodoProtagonista = nodoEscenario[i]["protagonista"];
						 tipoProtagonista = nodoProtagonista[0]["tipo"].as<std::string>();
					 }
				 }

			 }
	  }
    }
else{
	//Si esta mal formado el archivo YAML pasado como parametro, debe cargar archivo con configuracion inicial
	this->LoadDefaultConfiguration();
	}
    std::cout << "El tipo del protagonista es: " << tipoProtagonista << std::endl;
	std::cout << "La velocidad del personaje sera de: " << vel_personaje << std::endl;
	std::cout << "El margen de scroll sera de: " << margen_scroll << std::endl;
	std::cout << "Alto por defecto es: " << ALTO_PANTALLA_DEFAULT << std::endl;
	std::cout << "Ancho por defecto es: " << ANCHO_PANTALLA_DEFAULT << std::endl;
	std::cout << "El alto de la pantalla es: " << altoPantalla << std::endl;
	std::cout << "El ancho de la pantalla es: " << anchoPantalla << std::endl;

}

void GameConfiguration::LoadDefaultConfiguration(){
	this->ParseYAML(this->defaultFile);
	}*/

