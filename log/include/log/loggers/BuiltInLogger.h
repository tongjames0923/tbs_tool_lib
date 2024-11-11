//
// Created by abstergo on 2024/2/13.
//
#include "log/log.hpp"
#include <cstdio>
#include <fstream>
#ifndef TBS_CPP_BUILTIN_LOGGER_H
#define TBS_CPP_BUILTIN_LOGGER_H
namespace BuiltInLoggers {
class ConsoleLogger : public BaseLogger {
public:
  explicit ConsoleLogger(const tbs::str_type &name = "undefined");
  void log(const LogLevel &level, const char *str) const override;
private:
};

class SimpleFileLogger : public BaseLogger {
public:
  SimpleFileLogger(const tbs::str_type &name, const tbs::str_type &file);
  void log(const LogLevel &level, const char *str) const override;
  ~SimpleFileLogger() override;
private:
  tbs::str_type _name;
  mutable std::ofstream _file;
};

}

#endif
