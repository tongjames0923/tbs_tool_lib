//
// Created by abstergo on 24-11-11.
//
#include <time_utils.hpp>
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
#include <match/match_macro.h>

using namespace time_utils;
using namespace std;
using namespace BuiltInLoggers;

ConsoleLogger *consoleLogger = new ConsoleLogger("console");

LoggerWrapper<LogLevel::INFO> logger{consoleLogger};

LoggerWrapper<LogLevel::TRACE> tlogger{consoleLogger};

#define LOGGER_WRAPPER tlogger

#include <log/log_macro.h>

#include <concurrency/LockAdapter.h>
#include <concurrency/sync_point/SyncPoint.h>

using namespace tbs::concurrency;

#include <concurrency/adapters.h>
#include <latch>
#include <guard.h>
#include <concurrency/containers/ConcurrentQueue.h>
#define N 10

using cQ = containers::ConcurrentQueue<int, RecursiveLockAdapter>;

cQ q;

int main(int argc, char *argv[])
{
    thread t1(
            [&]()
            {
                for (int i = 0; i < N; ++i)
                {
                    this_thread::sleep_for(time_utils::ms(5000));
                    q.push(i);
                }
            });
    t1.detach();
    LOG_INFO("ready to begin");
    auto i = q.pop();
    LOG_INFO("got {}", i);
    cQ q2 = q;

    return 0;
}