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


LoggerWrapper<LogLevel::INFO> logger{consoleLogger};


LoggerWrapper<LogLevel::TRACE> tlogger{consoleLogger};


#define LOGGER_WRAPPER tlogger

#include <log/log_macro.h>

int main(int argc, char *argv[]) {
  cout << utils_now() << endl;

  LOG_INFO("hello world{}", 1);
  LOG_DEBUG("hello world{}", 2);
  LOG_TRACE("hello world{}", 3);

  LOG_ERROR("hello world{}", 4);
  LOG_WARN("hello world{}", 5);
  return 0;
}
