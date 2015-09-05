/*
 * Log.h
 *
 *  Created on: Sep 4, 2015
 *      Author: dario
 */

#ifndef UTILS_LOG_H_
#define UTILS_LOG_H_

#include <string>

class Log {
public:
	Log();

	//Message
	static void Message(std::string tag,std::string msg);
	//Debug
	static void Debug(std::string tag,std::string msg);
	//Warning
	static void Warning(std::string tag,std::string msg);
	//Error
	static void Error(std::string tag,std::string msg);

	virtual ~Log();
private:
	static void Save(std::string type, std::string tag,std::string msg);
};

#endif /* UTILS_LOG_H_ */
