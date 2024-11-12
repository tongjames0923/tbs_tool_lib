//
// Created by abstergo on 24-11-11.
//
#include <time_utils.hpp>
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
#include <match/matchers.h>

using namespace time_utils;
using namespace std;
using namespace BuiltInLoggers;

ConsoleLogger *consoleLogger = new ConsoleLogger("console");


LoggerWrapper<LogLevel::INFO> logger{consoleLogger};


LoggerWrapper<LogLevel::TRACE> tlogger{consoleLogger};


#define LOGGER_WRAPPER tlogger

#include <log/log_macro.h>


template<typename T, typename F>
auto match(T v, F f) {
  if (f.test(v)) {
	return f(v);
  }
  return v;
}

template<typename T, typename F, typename ...ARGS>
auto match(T v, F f, ARGS... args) {
  if (f.test(v)) {
	return f(v);
  }
  return match(v, args...);
}


template<typename T, typename ...ARGS>
auto match(T v, ARGS... args) {
  return match(v, args...);
}

int main(int argc, char *argv[]) {
  int target = 88;
  auto any = make_any_matcher([](auto t, auto v) -> auto {
	LOG_INFO("hello world");
	return 0;
  });
  auto equal = make_equal_matcher(32, [](int n, int t) -> int {
	LOG_INFO("equal able");
	return n * 10;
  });

  auto functional = make_functional_matcher([](int n, auto i) {
	LOG_INFO("functional able");
	return n * 100;
  }, [&]() {
	return target % 2 == 0;
  });
  auto k = match(target, functional, equal, any);
  cout << k << endl;
  return 0;
}
