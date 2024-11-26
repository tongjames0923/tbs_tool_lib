// Created by tongj on 2023/10/6.
//

#ifndef TBS_CPP_LOG_HPP
#define TBS_CPP_LOG_HPP

#include <format>
#include <tbs/defs.h>
#include <vector>

enum LogLevel {
  TRACE = 5,
  DEBUG = 4,
  INFO = 3,
  WARN = 2,
  ERROR = 1
};

/**
 * 日志输出的格式化宏
 */
#define LOG_FORMAT(...) std::format(__VA_ARGS__)

// 定义一个基础日志类，用于派生出具体的日志记录器
class BaseLogger {
public:
  // 构造函数，允许在创建日志对象时指定日志记录器的名称
  // 参数 name: 日志记录器的名称，默认为"undefined"
  explicit BaseLogger(const char *name = "undefined") : _name(name) {
  }

  // 获取日志记录器的名称
  // 返回值: 日志记录器的名称
  const tbs::str_type &getLoggerName() const {
	return _name;
  }

  // 纯虚函数，用于记录日志，具体的实现将在派生类中提供
  // 参数 level: 日志的级别
  // 参数 str: 要记录的日志信息
  virtual void log(const LogLevel &level, const char *str) const = 0;

  // 虚析构函数，确保派生类对象能够被正确地销毁
  virtual ~BaseLogger() = default;

private:
  // 存储日志记录器的名称
  tbs::str_type _name;
};

template<LogLevel limit>
class LoggerWrapper {
private:
  std::vector<BaseLogger *> _loggers;

  // 基本情况：没有参数
  void addLoggers() {}

  // 递归情况：处理一个参数并递归处理剩余参数
  template<typename T, typename... Args>
  void addLoggers(T &&logger, Args &&...rest) {
	if (logger == nullptr) {
	  throw std::invalid_argument("logger can not be null pointer");
	}
	_loggers.push_back(std::forward<T>(logger));
	addLoggers(std::forward<Args>(rest)...);
  }

public:

  template<class  ...Args>
  LoggerWrapper(Args &&...args) {
	// 使用模板递归将参数添加到 _loggers 中
	addLoggers(std::forward<Args>(args)...);
  }


  void info(const char *) CONST;


  void warn(const char *) CONST;


  void error(const char *) CONST;


  void debug(const char *) CONST;


  void trace(const char *) CONST;
};

template<LogLevel limit>
inline void LoggerWrapper<limit>::info(const char *str) const {
  if constexpr (limit >= LogLevel::INFO) {
	for (auto &_inused_logger_ : _loggers) {
	  _inused_logger_->log(LogLevel::INFO, str);
	}
  }
}

template<LogLevel limit>

inline void LoggerWrapper<limit>::warn(const char *stras) const {
  if constexpr (limit >= LogLevel::WARN) {
	for (auto &_inused_logger_ : _loggers) {
	  _inused_logger_->log(LogLevel::WARN, stras);
	}
  }
}

template<LogLevel limit>

inline void LoggerWrapper<limit>::error(const char *stras) const {
  if constexpr (limit >= LogLevel::ERROR) {
	for (auto &_inused_logger_ : _loggers) {
	  _inused_logger_->log(LogLevel::ERROR, stras);
	}
  }
}

template<LogLevel limit>

inline void LoggerWrapper<limit>::debug(const char *stras) const {
  if constexpr (limit >= LogLevel::DEBUG) {
	for (auto &_inused_logger_ : _loggers) {
	  _inused_logger_->log(LogLevel::DEBUG, stras);
	}
  }
}

template<LogLevel limit>

void LoggerWrapper<limit>::trace(const char *stras) const {
  if constexpr (limit >= LogLevel::TRACE) {
	for (auto &_inused_logger_ : _loggers) {
	  _inused_logger_->log(LogLevel::TRACE, stras);
	}
  }
}




#endif
