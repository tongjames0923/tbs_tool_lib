//
// Created by tongj on 2023/10/6.
//

#ifndef TBS_CPP_TIME_UTILS_HPP
#define TBS_CPP_TIME_UTILS_HPP

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <defs.h>


namespace time_utils
{

    using timeUnit = long long int;

    using ms = std::chrono::milliseconds;

/**
 * 系统标准的时间单位
 */
    using standard_time = ms;

    /**
     * 时间数据
     */
    struct DateTime
    {
        int year, month, dayOfMonth, hour, minutes, sec, ms;

        explicit DateTime(int y = 1900, int m = 0, int d = 1, int h = 0, int mm = 0, int SS = 0, int ss = 0)
                :year(y), month(m), dayOfMonth(d), hour(h), minutes(mm), sec(SS), ms(ss)
        {
        }
    };

    /**
     * 获取现在的时间戳
     * @return
     */
    static timeUnit utils_now()
    {
        return std::chrono::duration_cast<standard_time>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    template<class T> static auto as(timeUnit t)
    {
        return std::chrono::duration_cast<T>(standard_time(t)).count();
    }

    /**
     * 获取DateTime 时间戳
     * @param tus 单位毫秒
     * @return
     */
    static DateTime getDateTime(timeUnit tus)
    {
        time_t tms = std::chrono::system_clock::to_time_t(std::chrono::time_point<std::chrono::system_clock>(ms(tus)));
        tm* t = localtime(&tms);
        return DateTime((t->tm_year) + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tus % 1000);
    }

    /**
     * 根据时间获取时间戳
     * @param t 需要转化成时间戳的日期时间
     * @return
     */

    static timeUnit getTime(CONST DateTime& t)
    {
        using namespace std::chrono;
        using namespace std;
        tm tp;
        tp.tm_sec = t.sec;
        tp.tm_min = t.minutes;
        tp.tm_hour = t.hour;
        tp.tm_mday = t.dayOfMonth;
        tp.tm_mon = t.month - 1;
        tp.tm_year = t.year - 1900;
        tp.tm_isdst = -1;
        time_t ut = std::mktime(&tp);
        return duration_cast<ms>(seconds(ut)).count() + t.ms;
    }

    /**
     * 格式化时间戳,根据standard_time
     * @param t
     * @param format 默认格式：年-月-日 时:分:秒，详细参考https://zh.cppreference.com/w/cpp/chrono/c/strftime
     * @return
     */
    static tbs::str_type outputTime(timeUnit t, CONST char* format = "%Y-%m-%d %H:%M:%S")
    {
        time_t tms = std::chrono::system_clock::to_time_t(
                std::chrono::time_point<std::chrono::system_clock>(standard_time(t)));
        std::stringstream ss;
        ss << std::put_time(std::localtime(&tms), format);
        return ss.str();
    }

}

#endif // TBS_CPP_TIME_UTILS_HPP
