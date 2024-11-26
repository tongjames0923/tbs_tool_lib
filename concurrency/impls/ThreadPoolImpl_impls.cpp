//
// Created by abstergo on 24-11-21.
//


#ifndef THREADPOOL_THREADPOOLIMPL_IMPLS_H
#define THREADPOOL_THREADPOOLIMPL_IMPLS_H
#include <concurrency/adapters.h>
#include <concurrency/containers/ConcurrentPriorityQueue.h>
#include <tbs/log/loggers/BuiltInLogger.h>
#include <threads/ThreadPool.h>
namespace tbs::threads
{

    using namespace BuiltInLoggers;

    auto logger = BuiltInLoggers::ConsoleLogger("threadPool Impl");

#ifndef LOG_LEVEL
#define LOG_LEVEL LogLevel::INFO
#endif


    LoggerWrapper<(LogLevel)LOG_LEVEL> g({&logger});

#define LOGGER_WRAPPER g
#include <tbs/log/log_macro.h>

    class ThreadPoolImpl
    {
    private:
        ThreadPoolData _config;
        std::unordered_map<size_t, std::thread> _threads;
        std::vector<tbs::concurrency::containers::ConcurrentPriorityQueue<ThreadTask>> _tasks;
        ThreadPool* _pool;
        std::atomic_size_t _taskCount{0};
        SharedMutexLockAdapter locker;
        using lockIt = concurrency::guard::auto_op_lock_guard<SharedMutexLockAdapter>;

    public:
        ThreadPoolImpl(CONST ThreadPoolData& config, ThreadPool* pool) : _config{config}, _pool{pool}
        {
            _tasks.resize(config.threadCount);
        }
        CONST ThreadPoolData& config() CONST
        {
            return _config;
        }
        ThreadPoolData& config()
        {
            return _config;
        }
        void stop()
        {
            _config.running = false;
            for (auto& t : _threads)
            {
                t.second.join();
            }
        }

        void addTask(CONST ThreadTask& task)
        {
            if (!_config.running)
            {
                throw std::runtime_error("ThreadPool is not running");
            }
            size_t tc = ++_taskCount;
            if (tc >= _config.maxTaskCount * _config.threadCount)
            {
                if (_config.exceptionHandler != nullptr)
                {
                    std::runtime_error runtime_error("ThreadPool is full");
                    error_info er{threads::EXCEPTION_TASK_COUNT_FULL, &runtime_error};
                    ThreadTask t = task;
                    _config.exceptionHandler(&er, &_config, &t, _pool);
                    return;
                }
            }
            size_t index = (tc % _config.threadCount);
            createEnv(index, index);
            _tasks[index].push(task);
        }

        void createEnv(CONST size_t& beg, CONST size_t& end)
        {
            lockIt p(locker);
            for (size_t index = beg; index < end; index++)
            {
                if (!_threads.contains(index))
                {
                    _threads[index] = createNewThread(index);
                    LOG_INFO("create new thread  {}", index);
                    _threads[index].detach();
                }
            }
        }

        void eventTrigger(event_info& info)
        {
            if (_config.eventHandler != nullptr)
            {
                if (_config.eventHandler != nullptr)
                {
                    info.waitting_tasks = _taskCount;
                    _config.eventHandler(&info, &_config, _pool);
                }
            }
        }

        std::thread createNewThread(CONST size_t& i)
        {
            return std::thread(
                [this, i]()
                {
                    while (_config.running)
                    {
                        event_info ei{event_info::WAITTING, nullptr, i, _config.threadCount, _taskCount};
                        eventTrigger(ei);
                        auto taskOp = _tasks[i].poll(time_utils::ms(config().maxIdleTime));
                        if (!taskOp.has_value())
                        {
                            break;
                        }
                        ThreadTask t = taskOp.value();
                        ei.runningTask = &t;
                        ei.signal = event_info::PICKED;
                        eventTrigger(ei);
                        try
                        {
                            if (t.status == ThreadTask::CREATED)
                            {
                                t.status = ThreadTask::RUNNING;
                                ei.signal = event_info::RUNNING;
                                eventTrigger(ei);
                                t.task();
                                t.status = ThreadTask::FINISHED;
                                ei.signal = event_info::FINISHED;
                                eventTrigger(ei);
                            }
                            else if (t.status == ThreadTask::CANCELED)
                            {
                                ei.signal = event_info::CANCELED;
                                eventTrigger(ei);
                                continue;
                            }
                        }
                        catch (std::exception& ex)
                        {
                            error_info e{threads::EXCEPTION_TASK_ERROR, &ex, i};
                            if (_config.exceptionHandler != nullptr)
                            {
                                _config.exceptionHandler(&e, &_config, &t, _pool);
                            }
                        }
                        --_taskCount;
                        {
                            lockIt g(locker);
                            _threads.erase(i);
                        }
                    }
                });
        }

        void threadStart()
        {
            if (_config.running)
            {
                throw std::runtime_error("ThreadPool has running");
            }
            _config.running = true;
            createEnv(0, _config.threadCount);
        }
    };
} // namespace tbs::threads


#endif
