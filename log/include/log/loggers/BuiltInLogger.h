//
// Created by abstergo on 2024/2/13.
//
#include "log/log.hpp"
#include <cstdio>
#include <fstream>

// 防止重复定义的头文件保护标志
#ifndef TBS_CPP_BUILTIN_LOGGER_H
#define TBS_CPP_BUILTIN_LOGGER_H

// 命名空间用于封装内置的日志记录器
namespace BuiltInLoggers {

/**
 * @brief 控制台日志记录器类
 * 
 * 继承自BaseLogger，用于在控制台输出日志信息
 */
class ConsoleLogger : public BaseLogger {
public:
  /**
   * @brief 构造函数
   * 
   * @param name 日志记录器的名称，默认为"undefined"
   */
  explicit ConsoleLogger(const tbs::str_type &name = "undefined");

  /**
   * @brief 记录日志信息到控制台
   * 
   * @param level 日志级别
   * @param str 要记录的日志信息
   */
  void log(const LogLevel &level, const char *str) const override;
private:
};

/**
 * @brief 简单文件日志记录器类
 * 
 * 继承自BaseLogger，用于将日志信息输出到文件
 */
class SimpleFileLogger : public BaseLogger {
public:
  /**
   * @brief 构造函数
   * 
   * @param name 日志记录器的名称
   * @param file 日志文件的路径
   */
  SimpleFileLogger(const tbs::str_type &name, const tbs::str_type &file);

  /**
   * @brief 记录日志信息到文件
   * 
   * @param level 日志级别
   * @param str 要记录的日志信息
   */
  void log(const LogLevel &level, const char *str) const override;

  /**
   * @brief 析构函数
   * 
   * 确保文件资源在日志记录器被销毁时正确释放
   */
  ~SimpleFileLogger() override;
private:
  tbs::str_type _name; // 日志记录器的名称
  mutable std::ofstream _file; // 日志文件流对象
};

}

#endif