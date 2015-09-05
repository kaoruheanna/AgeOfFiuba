/*
 * Log.cpp
 *
 *  Created on: Sep 4, 2015
 *      Author: dario
 */

#include "Log.h"
#include <iostream>
#include <sstream>

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
	std::cout << TimeToString(time(0)) << "-" <<  type << "-[" << tag << "]- " << msg << std::endl;
}

time_t Log::GetExecutionTime() {
	static time_t executionTime;

	if (!executionTime) {
		executionTime = time(0);
	}
	return executionTime;
}

string Log::TimeToString(time_t time) {
	   tm *ltm = localtime(&time);

	   std::ostringstream oss;
	   oss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << " " << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec;

	   return oss.str();
}
