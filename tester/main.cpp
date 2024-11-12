//
// Created by abstergo on 24-11-11.
//
#include <time_utils.hpp>
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
#include <match/match_macro.h>

using namespace time_utils;
using namespace std;
using namespace BuiltInLoggers;

ConsoleLogger *consoleLogger = new ConsoleLogger("console");


LoggerWrapper<LogLevel::INFO> logger{consoleLogger};


LoggerWrapper<LogLevel::TRACE> tlogger{consoleLogger};


#define LOGGER_WRAPPER tlogger

#include <log/log_macro.h>


int main(int argc, char *argv[]) {
  int target = 4;
  auto any =
	  ANY_MATCH(
		  LOG_INFO("any mathched {} ", _target_);
	  );
  auto equal = EQUAL_MATCH(32, LOG_INFO("equal able"););
  auto functional = FUNCTION_MATCH(target % 2 == 0, LOG_INFO("functional able"););
  matchWithoutReturn(target, equal, functional, any);
  return 0;
}
