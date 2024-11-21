//
// Created by abstergo on 24-11-21.
//

#include <threads/ThreadPool.h>
#include "ThreadPoolImpl_impls.cpp"


namespace tbs::threads
{
    bool ThreadPool::isRunning() const
    {
        return getImpl().config().running;
    }
    void ThreadPool::start()
    {
        getImpl().threadStart();
    }
    ThreadPool::ThreadPool(const ThreadPoolData& config) : PointerImpl<tbs::threads::ThreadPoolImpl, Resetor>{new ThreadPoolImpl(config, this)}
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
