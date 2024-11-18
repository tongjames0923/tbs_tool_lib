//
// Created by abstergo on 24-11-11.
//
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
#include <match/match_macro.h>
#include <time_utils.hpp>

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
#include <concurrency/containers/ConcurrentPriorityQueue.h>
#include <concurrency/containers/ConcurrentQueue.h>
#include <guard.h>
#include <latch>
#define N 50
#define TN 2

using cQ = containers::ConcurrentPriorityQueue<int, vector<int>, std::less<int> >;

cQ q;

int main(int argc, char *argv[])
{
    for (int k = 0; k < TN; k++)
    {
        thread t1(
                [k]()
                {
                    for (int i = 0; i < N / TN; ++i)
                    {
                        LOG_INFO("push {} {}", k, i);
                        q.push(i);
                    }
                });
        t1.detach();
    }
    int c = 0;
    LOG_INFO("ready to begin");
    while (c < N)
    {
        const int &i = q.top();
        LOG_INFO("got {} {}", i, c);
        q.pop();
        c++;
    }

    return 0;
}