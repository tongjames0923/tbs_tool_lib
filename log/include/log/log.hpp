// Created by tongj on 2023/10/6.
//

#ifndef TBS_CPP_LOG_HPP
#define TBS_CPP_LOG_HPP

#include <format>
#include <defs.h>
#define L_TRACE 5
#define L_DEBUG 4
#define L_INFO 3
#define L_WARN 2
#define L_ERROR 1

#define L_OFF 0

#define LOG_LEVEL L_TRACE

enum LogLevel {
  TRACE = 5, DEBUG = 4, INFO = 3, WARN = 2, ERROR = 1
};

/**
 * 日志输出的格式化宏
 */
#define LOG_FORMAT(...) std::format(__VA_ARGS__)

class BaseLogger {
public:
  explicit BaseLogger(const char *name = "undefined") : _name(name) {
  }

  [[nodiscard]] const tbs::str_type &getLoggerName() const {
	return _name;
  }

  virtual void log(const LogLevel &level, const char *str) const = 0;
  virtual ~BaseLogger() = default;

private:
  tbs::str_type _name;
};

#if LOG_LEVEL >= L_TRACE
#define TRACE_LOG(...)                                                                                                 \
    {                                                                                                                  \
                                                                                                                       \
        for (auto& _inused_logger_ : std::initializer_list<BaseLogger*>{INUSE_LOGGERS})                                \
        {                                                                                                              \
            _inused_logger_->log(LogLevel::TRACE, LOG_FORMAT(__VA_ARGS__).c_str());                                   \
        }                                                                                                              \
    }

#else
#define TRACE_LOG(...)
#endif

#if LOG_LEVEL >= L_DEBUG
#define DEBUG_LOG(...)                                                                                                 \
    {                                                                                                                  \
                                                                                                                       \
        for (auto& _inused_logger_ : std::initializer_list<BaseLogger*>{INUSE_LOGGERS})                                \
        {                                                                                                              \
            _inused_logger_->log(LogLevel::DEBUG, LOG_FORMAT(__VA_ARGS__).c_str());                                   \
        }                                                                                                              \
    }

#else
#define DEBUG_LOG(...)
#endif

#if LOG_LEVEL >= L_INFO
#define INFO_LOG(...)                                                                                                  \
    {                                                                                                                  \
                                                                                                                       \
        for (auto& _inused_logger_ : std::initializer_list<BaseLogger*>{INUSE_LOGGERS})                                \
        {                                                                                                              \
            _inused_logger_->log(LogLevel::INFO, LOG_FORMAT(__VA_ARGS__).c_str());                                    \
        }                                                                                                              \
    }

#else
#define INFO_LOG(...)
#endif

#if LOG_LEVEL >= L_WARN
#define WARN_LOG(...)                                                                                                  \
    {                                                                                                                  \
                                                                                                                       \
        for (auto& _inused_logger_ : std::initializer_list<BaseLogger*>{INUSE_LOGGERS})                                \
        {                                                                                                              \
            _inused_logger_->log(LogLevel::WARN, LOG_FORMAT(__VA_ARGS__).c_str());                                    \
        }                                                                                                              \
    }

#else
#define WARN_LOG(...)
#endif

#if LOG_LEVEL >= L_ERROR
#define ERROR_LOG(...)                                                                                                 \
    {                                                                                                                  \
                                                                                                                       \
        for (auto& _inused_logger_ : std::initializer_list<BaseLogger*>{INUSE_LOGGERS})                                \
        {                                                                                                              \
            _inused_logger_->log(LogLevel::ERROR, LOG_FORMAT(__VA_ARGS__).c_str());                                   \
        }                                                                                                              \
    }

#else
#define ERROR_LOG(...)
#endif

#endif// TBS_CPP_LOG_HPP
