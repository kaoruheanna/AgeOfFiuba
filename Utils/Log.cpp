/*
 * Log.cpp
 *
 *  Created on: Sep 4, 2015
 *      Author: dario
 */

#include "Log.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

Log::Log() {
	// TODO Auto-generated constructor stub
	Init();
	messageLevel = logDEBUG;
}

void Log::Init() {
	static time_t executionTime;

	if (!executionTime) {
		executionTime = time(0);
	}
	tm *ltm = localtime(&executionTime);
	std::ostringstream oss;
	oss << 1900 + ltm->tm_year << 1 + ltm->tm_mon << ltm->tm_mday << "_" << ltm->tm_hour << ":" << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec;


	fout.open(oss.str().c_str(), ios::out | ios::app);
}

Log::~Log()
{
	/*
   if (messageLevel <= Log::ReportingLevel())
   {
      os << std::endl;
      fprintf(stderr, "%s", os.str().c_str());
      fflush(stderr);
   }*/
	cout << os.str() << endl;
	if (fout.is_open()) {
		fout << os.str() << "\n";
		fout.close();
	}
}

std::ostringstream& Log::Get(TLogLevel level)
{
	os << TimeToString(NowTime()) << " - ";
	//os << " " << ToString(level) << ": ";
	//os << std::string(level > logDEBUG ? 0 : level - logDEBUG, '\t');
	messageLevel = level;
	return os;
}

TLogLevel Log::ReportingLevel() {
	return logINFO;
}
time_t Log::NowTime() {
	return time(0);
}

string Log::TimeToString(time_t time) {
	   tm *ltm = localtime(&time);

	   std::ostringstream oss;
	   oss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << " " << ltm->tm_hour << ":" << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec;

	   return oss.str();
}

