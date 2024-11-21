//
// Created by abstergo on 24-11-11.
//
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
#include <match/match_macro.h>
#include <time_utils.hpp>
#include <PointerToImpl.h>
#include <concurrency/sync_point/SyncPoint.h>
#include <log/log_macro.h>

auto logger = BuiltInLoggers::ConsoleLogger("main");

LoggerWrapper<LogLevel::INFO> g({&logger});

#define LOGGER_WRAPPER g

#include <log/log_macro.h>


int main(int argc, char *argv[])
{
    using namespace tbs::concurrency::sync_point;
    SyncPoint sp;
    for (int i = 0; i < 5; i++)
    {
        std::thread t(
                [&sp, i]()
                {
                    LOG_INFO("thread {} start", i);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300 * i));
                    sp.accumulateFlag(1);
                });
        t.detach();
    }
    LOG_INFO("main thread start");
    sp.wait_flag(5);
    LOG_INFO("main thread end");

    return 0;
}