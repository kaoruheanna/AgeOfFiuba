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
	oss  << "Logs/" << 1900 + ltm->tm_year << 1 + ltm->tm_mon << ltm->tm_mday << "_" << ltm->tm_hour << ":" << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec;


	fout.open(oss.str().c_str(), ios::out | ios::app);
}

Log::~Log()
{
	cout << os.str() << endl;
	if (fout.is_open() && (messageLevel < Log::ReportingLevel())) {
		fout << os.str() << "\n";
		fout.close();
	}
}

std::ostringstream& Log::Get(string tag, TLogLevel level)
{
	this->tag = tag;
	os << TimeToString(NowTime()) << " - ";
	os << GetTextForEnum(level) << " - " << "{" << this->tag << "} - ";
	messageLevel = level;
	return os;
}

TLogLevel Log::ReportingLevel() {
	return logDEBUG;
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
