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

#include <concurrency/LockAdapter.h>


using namespace tbs::concurrency;

#include <concurrency/lock_operators/operators.h>
#include <latch>

mutex_lock_adapter l;

#define N 200

int main(int argc, char *argv[]) {
  int k = 0;
  std::latch lat(N);
  for (int i = 0; i < N; i++) {
	thread r([&]() {
	  l.lock();
	  std::cout << "thread " << k++ << std::endl;
	  l.unlock();
	  lat.count_down();
	});
	r.detach();
  }
  lat.wait();
  return 0;
}
