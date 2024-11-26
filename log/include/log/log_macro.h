//
// Created by abstergo on 24-11-12.
//

// 解除已定义的日志级别宏定义，以防止重复定义错误
#undef LOG_INFO
#undef LOG_WARN
#undef LOG_ERROR
#undef LOG_DEBUG
#undef LOG_TRACE

// 包含日志库的头文件
#include <tbs/log/log.hpp>

// 根据是否定义了 LOGGER_WRAPPER 宏来定义不同的日志级别宏
#ifndef LOGGER_WRAPPER
// 如果没有定义 LOGGER_WRAPPER，使用传入的 wrapper 对象进行日志记录
#define LOG_INFO(wrapper, ...) wrapper.info(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 INFO 级别的日志
/// @param wrapper 日志记录器对象
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_WARN(wrapper, ...) wrapper.warn(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 WARN 级别的日志
/// @param wrapper 日志记录器对象
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_ERROR(wrapper, ...) wrapper.error(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 ERROR 级别的日志
/// @param wrapper 日志记录器对象
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_DEBUG(wrapper, ...) wrapper.debug(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 DEBUG 级别的日志
/// @param wrapper 日志记录器对象
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_TRACE(wrapper, ...) wrapper.trace(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 TRACE 级别的日志
/// @param wrapper 日志记录器对象
/// @param ... 可变参数列表，用于格式化日志消息

#else
// 如果定义了 LOGGER_WRAPPER，使用全局的 LOGGER_WRAPPER 对象进行日志记录
#define LOG_INFO(...) LOGGER_WRAPPER.info(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 INFO 级别的日志
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_WARN(...) LOGGER_WRAPPER.warn(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 WARN 级别的日志
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_ERROR(...) LOGGER_WRAPPER.error(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 ERROR 级别的日志
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_DEBUG(...) LOGGER_WRAPPER.debug(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 DEBUG 级别的日志
/// @param ... 可变参数列表，用于格式化日志消息

#define LOG_TRACE(...) LOGGER_WRAPPER.trace(LOG_FORMAT(__VA_ARGS__).c_str())
/// @brief 记录 TRACE 级别的日志
/// @param ... 可变参数列表，用于格式化日志消息
#endif

// 如果定义了 LOGGER_WRAPPER，解除其定义
#ifdef LOGGER_WRAPPER
#undef LOGGER_WRAPPER
#endif
