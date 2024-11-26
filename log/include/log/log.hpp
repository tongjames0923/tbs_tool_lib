// Created by tongj on 2023/10/6.
//

#ifndef TBS_CPP_LOG_HPP
#define TBS_CPP_LOG_HPP

#include <format>
#include <tbs/defs.h>
#include <vector>

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

/**
 * @brief 日志级别枚举
 *
 * 用于表示不同级别的日志信息，从最详细的 TRACE 到最不详细的 NONE。
 */
enum LogLevel
{
    TRACE = 5, ///< 最详细的日志级别，通常用于调试
    DEBUG = 4, ///< 用于调试信息
    INFO = 3, ///< 一般信息，如程序启动、停止等
    WARN = 2, ///< 警告信息，表示可能的问题
    ERROR = 1, ///< 错误信息，表示程序运行中的错误
    NONE = 0 ///< 不记录任何日志
};

/**
 * @brief 日志输出的格式化宏
 *
 * 使用 std::format 进行日志信息的格式化。
 */
#define LOG_FORMAT(...) std::format(__VA_ARGS__)

/**
 * @brief 基础日志类
 *
 * 用于派生出具体的日志记录器。提供了基本的日志记录功能和日志记录器的名称管理。
 */
class BaseLogger
{
public:
    /**
     * @brief 构造函数
     *
     * 允许在创建日志对象时指定日志记录器的名称。
     *
     * @param name 日志记录器的名称，默认为 "undefined"
     */
    explicit BaseLogger(const char* name = "undefined") : _name(name)
    {
    }

    /**
     * @brief 获取日志记录器的名称
     *
     * @return 日志记录器的名称
     */
    const tbs::str_type& getLoggerName() const
    {
        return _name;
    }

    /**
     * @brief 记录日志
     *
     * 纯虚函数，用于记录日志，具体的实现将在派生类中提供。
     *
     * @param level 日志的级别
     * @param str 要记录的日志信息
     */
    virtual void log(const LogLevel& level, const char* str) const = 0;

    /**
     * @brief 虚析构函数
     *
     * 确保派生类对象能够被正确地销毁。
     */
    virtual ~BaseLogger() = default;

private:
    /**
     * @brief 存储日志记录器的名称
     */
    tbs::str_type _name;
};

/**
 * @brief 日志包装器类
 *
 * 用于管理多个日志记录器，并根据日志级别决定是否记录日志。
 *
 * @tparam limit 日志级别限制，只有大于或等于此级别的日志才会被记录
 */
template <LogLevel limit>
class LoggerWrapper
{
private:
    /**
     * @brief 存储多个日志记录器的指针
     */
    std::vector<BaseLogger*> _loggers;

    /**
     * @brief 基本情况：没有参数
     */
    void addLoggers()
    {
    }

    /**
     * @brief 递归情况：处理一个参数并递归处理剩余参数
     *
     * @tparam T 第一个参数的类型
     * @tparam Args 剩余参数的类型
     * @param logger 第一个参数，必须是非空指针
     * @param rest 剩余参数
     */
    template <typename T, typename... Args>
    void addLoggers(T&& logger, Args&&... rest)
    {
        if (logger == nullptr)
        {
            throw std::invalid_argument("logger can not be null pointer");
        }
        _loggers.push_back(std::forward<T>(logger));
        addLoggers(std::forward<Args>(rest)...);
    }

public:
    /**
     * @brief 构造函数
     *
     * 使用模板递归将参数添加到 _loggers 中。
     *
     * @tparam Args 参数的类型
     * @param args 可变数量的参数，每个参数都是一个 BaseLogger 的指针
     */
    template <class... Args>
    LoggerWrapper(Args&&... args)
    {
        addLoggers(std::forward<Args>(args)...);
    }

    /**
     * @brief 记录 INFO 级别的日志
     *
     * @param str 要记录的日志信息
     */
    void info(const char* str) const;

    /**
     * @brief 记录 WARN 级别的日志
     *
     * @param str 要记录的日志信息
     */
    void warn(const char* str) const;

    /**
     * @brief 记录 ERROR 级别的日志
     *
     * @param str 要记录的日志信息
     */
    void error(const char* str) const;

    /**
     * @brief 记录 DEBUG 级别的日志
     *
     * @param str 要记录的日志信息
     */
    void debug(const char* str) const;

    /**
     * @brief 记录 TRACE 级别的日志
     *
     * @param str 要记录的日志信息
     */
    void trace(const char* str) const;
};

/**
 * @brief 记录 INFO 级别的日志
 *
 * @param str 要记录的日志信息
 */
template <LogLevel limit>
inline void LoggerWrapper<limit>::info(const char* str) const
{
    if constexpr (limit >= LogLevel::INFO)
    {
        for (auto& _inused_logger_ : _loggers)
        {
            _inused_logger_->log(LogLevel::INFO, str);
        }
    }
}

/**
 * @brief 记录 WARN 级别的日志
 *
 * @param str 要记录的日志信息
 */
template <LogLevel limit>
inline void LoggerWrapper<limit>::warn(const char* str) const
{
    if constexpr (limit >= LogLevel::WARN)
    {
        for (auto& _inused_logger_ : _loggers)
        {
            _inused_logger_->log(LogLevel::WARN, str);
        }
    }
}

/**
 * @brief 记录 ERROR 级别的日志
 *
 * @param str 要记录的日志信息
 */
template <LogLevel limit>
inline void LoggerWrapper<limit>::error(const char* str) const
{
    if constexpr (limit >= LogLevel::ERROR)
    {
        for (auto& _inused_logger_ : _loggers)
        {
            _inused_logger_->log(LogLevel::ERROR, str);
        }
    }
}

/**
 * @brief 记录 DEBUG 级别的日志
 *
 * @param str 要记录的日志信息
 */
template <LogLevel limit>
inline void LoggerWrapper<limit>::debug(const char* str) const
{
    if constexpr (limit >= LogLevel::DEBUG)
    {
        for (auto& _inused_logger_ : _loggers)
        {
            _inused_logger_->log(LogLevel::DEBUG, str);
        }
    }
}

/**
 * @brief 记录 TRACE 级别的日志
 *
 * @param str 要记录的日志信息
 */
template <LogLevel limit>
void LoggerWrapper<limit>::trace(const char* str) const
{
    if constexpr (limit >= LogLevel::TRACE)
    {
        for (auto& _inused_logger_ : _loggers)
        {
            _inused_logger_->log(LogLevel::TRACE, str);
        }
    }
}

#endif // TBS_CPP_LOG_HPP
