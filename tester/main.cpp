//
// Created by abstergo on 24-11-11.
//
#include <tbs/concurrency/sync_point/SyncPoint.h>
#include <iostream>
#include <tbs/PointerToImpl.h>
#include <tbs/log/log.hpp>
#include <tbs/log/loggers/BuiltInLogger.h>
#include <tbs/threads/ThreadPool.h>
#include <tbs/time_utils.hpp>
using namespace tbs::threads;

auto logger = BuiltInLoggers::ConsoleLogger("main");

LoggerWrapper<LogLevel::INFO> g({&logger});

#define TASK 16
#define LOGGER_WRAPPER g
#include <tbs/log/log_macro.h>
int main(int argc, char* argv[])
{
    using namespace tbs::concurrency::sync_point;
    SyncPoint sp;
    ThreadPool pool(ThreadPoolData{"mainTask",
                                   8,
                                   8,
                                   false,
                                   [&sp](error_info* a, auto b, auto c, auto d)
                                   {
                                       sp.accumulateFlag(1);
                                       LOG_ERROR("error: {}", a->exception->what());
                                   },
                                   [](event_info* ei, ThreadPoolData* config, ThreadPool* pool)
                                   {
                                       switch (ei->signal)
                                       {
                                       case event_info::WAITTING:
                                           LOG_INFO("waiting at {}", ei->current_thread);
                                           break;
                                       case event_info::PICKED:
                                           LOG_INFO("picked at {}", ei->current_thread);
                                           break;
                                       case event_info::RUNNING:
                                           LOG_INFO("running at {}", ei->current_thread);
                                           break;
                                       case event_info::FINISHED:
                                           LOG_INFO("finished at {}", ei->current_thread);
                                           break;
                                       case event_info::CANCELED:
                                           LOG_INFO("canceled at {}", ei->current_thread);
                                           break;
                                       default:
                                           break;
                                       }
                                   }});
    pool.start();

    for (int i = 0; i < TASK; i++)
    {
        pool.submit([&sp]() { sp.accumulateFlag(1); });
    }
    LOG_INFO("main thread start");
    sp.wait_flag(TASK);
    LOG_INFO("main thread end");

    return 0;
}
