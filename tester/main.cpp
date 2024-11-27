//
// Created by abstergo on 24-11-11.
//
#include <iostream>
#include <tbs/PointerToImpl.h>
#include <tbs/concurrency/sync_point/SyncPoint.h>
#include <tbs/format.h>
#include <tbs/log/log.hpp>
#include <tbs/log/log_macro.h>
#include <tbs/log/loggers/BuiltInLogger.h>
#include <tbs/threads/ThreadPool.h>
#include <tbs/time_utils.hpp>
using namespace tbs::threads;

auto logger = BuiltInLoggers::ConsoleLogger("main");

LoggerWrapper<LogLevel::INFO> g({&logger});

#define TASK 16
#define LOGGER_WRAPPER g
#include <tbs/log/log_macro.h>


struct point
{
    int x;
    int y;
};

FORMATTER_REGISTER_QUICK(point, "x:{} y:{}", s.x, s.y)

int main(int argc, char* argv[])
{
    std::cout << format_string("helloworld {}", point{1, 2});

    // using namespace tbs::concurrency::sync_point;
    // SyncPoint sp;
    // tbs::threads::ThreadPool pool("pool", 8, 16, 2, 1000);
    // pool.start();
    //
    // std::cout << format_string("{}", pool);

    return 0;
}
