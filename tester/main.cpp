//
// Created by abstergo on 24-11-11.
//
#include <time_utils.hpp>
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
using namespace time_utils;
using namespace std;
using namespace BuiltInLoggers;

ConsoleLogger *consoleLogger = new ConsoleLogger("console");
#include <log/log.hpp>




#define INUSE_LOGGERS consoleLogger
int main(int argc, char *argv[])
{
	cout << utils_now() << endl;
	cout << LOG_LEVEL << endl;
	INFO_LOG("this is info log");
	DEBUG_LOG("this is debug log");
	return 0;
}
