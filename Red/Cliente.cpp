/*
 * Cliente.cpp
 *
 *  Created on: Oct 3, 2015
 *      Author: nico
 */

#include "Cliente.h"

#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "SerializableTest/DoubleStringSerializable.h"

using namespace std;


Cliente::Cliente() {
	// TODO Auto-generated constructor stub

}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

void Cliente::empezar(char* ip, int port) {
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("Cliente - Fallo el socket con error %i\n", sd);
		return;
	}
	sockaddr_in addr;
	bzero((char *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	struct hostent *ptr_srv;
	ptr_srv = gethostbyname(ip);
	if(ptr_srv == NULL){ // TODO castear a char* ?
		printf("Cliente - Fallo el host name\n");
		return; // ERR: -2
	}

	memcpy(&addr.sin_addr,
			ptr_srv->h_addr_list[0],
			ptr_srv->h_length);

	/*int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		return; // ERR: -1
	}*/
	// sd == sockfd

	if(connect(sd, (struct sockaddr *)&addr, sizeof(addr)) == -1){
		printf("Cliente - Fallo la coneccion\n");
		close(sd);
		return ; // ERR: -1
	}

	printf("Escribir algo: ");
	string testInput;
	getline(cin, testInput);
	/*
	 * Ejemplo de serializar un string
	printf("Cliente - Enviando %s ...", testInput.c_str());

	StringSerializable* serializable = new StringSerializable(testInput.c_str());
	int resultado = enviarSerializable(sd, serializable);
	printf("Cliente - Enviado con estado: %i\n", resultado);
	delete serializable;
	*/
	printf("Escribir algo mas: ");
	string secondTestInput;
	getline(cin, secondTestInput);

	// Ejemplo de serializar una estructura con 2 strings
	DoubleStringSerializable* serializable = new DoubleStringSerializable(testInput.c_str(), secondTestInput.c_str());
	int resultado = enviarSerializable(sd, serializable);
	printf("Cliente - Enviado con estado: %i\n", resultado);
	delete serializable;
	// TODO read / write

	printf("Cliente - Todo ok\n");
	close(sd);
}
