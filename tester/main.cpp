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

#include <concurrency/containers/ConcurrentUnorderedMap.h>

tbs::concurrency::containers::ConcurrentUnorderedMap<int, int> mp;
sync_point::SyncPoint                                          s;
int                                                            main(int argc, char *argv[])
{
    for (int i = 0; i < TN; i++)
    {
        thread t(
                [i]()
                {
                    for (int j = 0; j < N/TN; j++)
                    {
                        mp.operateIfExists(
                                i, [](int &v, auto &m) { ++v; },
                                [i](std::unordered_map<int, int> &m) { m.insert({i, 0}); });
                    }
                    s.accumulateFlag(1);
                });
        t.detach();
    }
    s.wait_flag(TN);
    mp.foreach (
            [](CONST pair<int, int> &p)
            {
                cout << p.first << ":" << p.second << endl;
                return true;
            });
    return 0;
}