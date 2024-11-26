//
// Created by abstergo on 24-11-12.
//

#undef LOG_INFO
#undef LOG_WARN
#undef LOG_ERROR
#undef LOG_DEBUG
#undef LOG_TRACE

#include <tbs/log/log.hpp>

#ifndef LOGGER_WRAPPER
#define LOG_INFO(wrapper, ...) wrapper.info(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_WARN(wrapper, ...) wrapper.warn(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_ERROR(wrapper, ...) wrapper.error(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_DEBUG(wrapper, ...) wrapper.debug(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_TRACE(wrapper, ...) wrapper.trace(LOG_FORMAT(__VA_ARGS__).c_str())

#else
#define LOG_INFO( ...) LOGGER_WRAPPER.info(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_WARN(...) LOGGER_WRAPPER.warn(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_ERROR(...) LOGGER_WRAPPER.error(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_DEBUG(...) LOGGER_WRAPPER.debug(LOG_FORMAT(__VA_ARGS__).c_str())
#define LOG_TRACE(...) LOGGER_WRAPPER.trace(LOG_FORMAT(__VA_ARGS__).c_str())
#endif
