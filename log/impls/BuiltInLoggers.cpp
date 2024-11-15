//
// Created by abstergo on 2024/2/14.
//
#include <log/log.hpp>
#include <log/loggers/BuiltInLogger.h>
#include <format>
#include <time_utils.hpp>

static const char *logLevelToString[] = {"UNKNOWN", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"};
static const char *logLevelToColor[] = {
	"\x1B[31m", // 红色
	"\x1B[33m", // 黄色
	"\x1B[32m", // 绿色
	"\x1B[36m", // 青色
	"\x1B[34m" // 蓝色
};
static std::mutex _log_mx;

void BuiltInLoggers::ConsoleLogger::log(const LogLevel &level, const char *str) const {
	auto now = time_utils::utils_now();
	tbs::sys_unique_lock g(_log_mx);
	printf("%s", logLevelToColor[level - 1]);
	printf("%s", std::format("{} {}:  CONTENT : ", getLoggerName(), logLevelToString[level]).c_str()); //输出日志级别
	printf("%s", "\x1B[4m");
	printf("%s", str); //输出日志内容
	printf("\x1B[0m %s %s",
	       logLevelToColor[level - 1],
	       std::format(" ;LOG_TIME: [ {}.{} ];",
	                   time_utils::outputTime(now),
	                   (double)(time_utils::as<time_utils::ms>(now) % 1000) / 1000.0 * 1000).c_str()); //输出日志时间
	printf("\n\x1B[0m"); //还原日志颜色并换行
	fflush(stdout);
}

BuiltInLoggers::ConsoleLogger::ConsoleLogger(const std::string &name)
	: BaseLogger(name.c_str()) {
}

BuiltInLoggers::SimpleFileLogger::SimpleFileLogger(const std::string &name, const std::string &file)
	: BaseLogger(name.c_str()) {
	_file.open(file, std::ios_base::app);
	if (!_file.is_open()) {
		throw tbs::base_error(std::format("can not open file :{}", file));
	}
}

void BuiltInLoggers::SimpleFileLogger::log(const LogLevel &level, const char *str) const {
	tbs::sys_unique_lock g(_log_mx);
	auto now = time_utils::utils_now();
	_file << std::format("At:{}.{:04d} {} [{}] Content: {} ;\n",
	                     time_utils::outputTime(now),
	                     std::chrono::duration_cast<std::chrono::milliseconds>(time_utils::standard_time(now)).count()
	                     % 1000,
	                     _name,
	                     logLevelToString[level],
	                     str);
}

BuiltInLoggers::SimpleFileLogger::~SimpleFileLogger() {
	_file << "logger closed \n\n";
	_file.close();
}
