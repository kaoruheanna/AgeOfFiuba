/*
 * Log.cpp
 *
 *  Created on: Sep 4, 2015
 *      Author: dario
 */

#include "Log.h"
#include <iostream>

Log::Log() {
	// TODO Auto-generated constructor stub

}

Log::~Log() {
	// TODO Auto-generated destructor stub
}

void Log::Message(std::string tag,std::string msg) {
	Save("[Message]",tag,msg);
}

void Log::Debug(std::string tag,std::string msg) {
	Save("[Debug]",tag,msg);
}

void Log::Warning(std::string tag,std::string msg) {
	Save("[Warning]",tag,msg);
}

void Log::Error(std::string tag,std::string msg) {
	Save("[Error]",tag,msg);
}

void Log::Save(std::string type, std::string tag,std::string msg) {
	std::cout <<  type << "-> " << tag << "," << msg << std::endl;
}
