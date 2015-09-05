/*
 * Log.h
 *
 *  Created on: Sep 4, 2015
 *      Author: dario
 */

#ifndef UTILS_LOG_H_
#define UTILS_LOG_H_

#include <string>
#include <ctime>

using namespace std;

class Log {
public:
	Log();

	//Message
	static void Message(string tag,string msg);
	//Debug
	static void Debug(string tag,string msg);
	//Warning
	static void Warning(string tag,string msg);
	//Error
	static void Error(string tag,string msg);

	virtual ~Log();
private:
	static time_t GetExecutionTime();
	static string TimeToString(time_t time);
	static void Save(std::string type, std::string tag,std::string msg);
};

#endif /* UTILS_LOG_H_ */
