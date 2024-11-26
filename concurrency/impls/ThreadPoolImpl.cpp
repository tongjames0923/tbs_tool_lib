//
// Created by abstergo on 24-11-21.
//

#include <threads/ThreadPool.h>

#include <utility>
#include "ThreadPoolImpl_impls.cpp"


namespace tbs::threads
{
    bool ThreadPool::isRunning() const
    {
        return getImpl().config().running;
    }
    const ThreadPoolData& ThreadPool::getConfig() const
    {
        return getImpl().config();
    }
    ThreadPoolData& ThreadPool::getConfig()
    {
        return getImpl().config();
    }
    void ThreadPool::start()
    {
        getImpl().threadStart();
    }
    ThreadPool::ThreadPool(const char* threadPoolName,
                           size_t threadCount,
                           size_t maxTaskCount,
                           size_t maxIdleThreadCount,
                           size_t maxIdleTime,
                           exception_handler exceptionHandler,
                           thread_pool_event_handler eventHandler) :
        PointerImpl<tbs::threads::ThreadPoolImpl, Resetor>(new ThreadPoolImpl(
            ThreadPoolData{threadPoolName, threadCount, maxTaskCount, false, std::move(exceptionHandler), std::move(eventHandler), maxIdleThreadCount, maxIdleTime}, this))
    {
    }
    void ThreadPool::stop()
    {
        getImpl().stop();
    }
    void ThreadPool::submit(std::function<void()> f, int priority)
    {
        getImpl().addTask(ThreadTask{f, ThreadTask::CREATED, priority});
    }

} // namespace tbs::threads
