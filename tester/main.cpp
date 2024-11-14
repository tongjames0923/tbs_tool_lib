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

#include <concurrency/adapters.h>
#include <latch>
#include <guard.h>

#define N 200

class A {
public:
  int a;

  A() : a(0) {}

  ~A() {
	LOG_INFO("destructor called");
  }
};

TimedMutexLockAdapter l;
atomic_bool flag = false;

int main(int argc, char *argv[]) {

  LOG_INFO("start");
  thread t1([&]() {
	guard::auto_op_lock_guard<decltype(l)> g(l);
	flag = true;
	this_thread::sleep_for(chrono::milliseconds(500));
	LOG_INFO("will release");
  });
  t1.detach();
  while (!flag) {
	this_thread::yield();
  }
  bool f = l.try_lock(1000);

  LOG_INFO("try_lock result: {}", f ? "true" : "false");
  if (f) {
	l.unlock();
  }
  l.lock();
  LOG_INFO("locked");
  l.unlock();

  return 0;
}
