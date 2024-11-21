//
// Created by abstergo on 24-11-21.
//


#ifndef THREADPOOL_THREADPOOLIMPL_IMPLS_H
#define THREADPOOL_THREADPOOLIMPL_IMPLS_H
#include <concurrency/containers/ConcurrentPriorityQueue.h>
#include <threads/ThreadPool.h>

namespace tbs::threads
{
    class ThreadPoolImpl
    {
    private:
        ThreadPoolData _config;
        std::vector<std::thread> _threads;
        std::vector<tbs::concurrency::containers::ConcurrentPriorityQueue<ThreadTask>> _tasks;
        ThreadPool* _pool;
        std::atomic_size_t _taskCount{0};


    public:
        ThreadPoolImpl(CONST ThreadPoolData& config, ThreadPool* pool) : _config{config}, _pool{pool}
        {
            _tasks.resize(config.threadCount);
        }
        CONST ThreadPoolData& config() CONST
        {
            return _config;
        }
        void stop()
        {
            _config.running = false;
            for (auto& t : _threads)
            {
                t.join();
            }
        }

        void addTask(CONST ThreadTask& task)
        {
            if (!_config.running)
            {
                throw std::runtime_error("ThreadPool is not running");
            }

            if (_taskCount >= _config.maxTaskCount)
            {
                if (_config.handler != nullptr)
                {
                    std::runtime_error runtime_error("ThreadPool is full");
                    error_info er{threads::EXCEPTION_TASK_COUNT_FULL, &runtime_error};
                    ThreadTask t = task;
                    _config.handler(&er, &_config, &t, _pool);
                    return;
                }
            }
            _tasks[(++_taskCount % _config.threadCount)].push(task);
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

        void threadStart()
        {
            if (_config.running)
            {
                throw std::runtime_error("ThreadPool is running");
                return;
            }
            _config.running = true;
            for (size_t i = 0; i < _config.threadCount; i++)
            {
                _threads.emplace_back(
                    [this, i]()
                    {
                        while (_config.running && i < _config.threadCount)
                        {
                            event_info ei{event_info::WAITTING, nullptr, i, _config.threadCount, _taskCount};
                            eventTrigger(ei);
                            ThreadTask t = _tasks[i].poll();
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
                                if (_config.handler != nullptr)
                                {
                                    _config.handler(&e, &_config, &t, _pool);
                                }
                            }
                            --_taskCount;
                        }
                    });
            }
        }
    };
} // namespace tbs::threads


#endif
