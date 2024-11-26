//
// Created by abstergo on 24-11-21.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <tbs/PointerToImpl.h>
#include <functional>
namespace tbs::threads
{

    constexpr static int EXCEPTION_TASK_COUNT_FULL = -1;
    constexpr static int EXCEPTION_TASK_ERROR = -2;

    class ThreadPool;
    struct ThreadPoolData;
    struct error_info;
    struct ThreadTask;
    struct event_info;


    using exception_handler = std::function<void(error_info* e, ThreadPoolData* config, ThreadTask* task, ThreadPool* pool)>;

    using thread_pool_event_handler = std::function<void(event_info* info, ThreadPoolData* config, ThreadPool* pool)>;

    struct event_info
    {
        constexpr static int WAITTING = 0;
        constexpr static int PICKED = 1;
        constexpr static int RUNNING = 2;
        constexpr static int FINISHED = 3;
        constexpr static int CANCELED = -1;

        int signal;
        ThreadTask* runningTask;
        size_t current_thread;
        size_t total_thread;
        size_t waitting_tasks;
    };

    struct error_info
    {
        int signal;
        std::exception* exception;
        size_t threadIndex;
    };

    struct ThreadPoolData
    {
        const char* threadPoolName;
        size_t threadCount;
        size_t maxTaskCount;
        bool running;
        exception_handler handler = nullptr;
        thread_pool_event_handler eventHandler = nullptr;
    };

    struct ThreadTask
    {
        constexpr static int RUNNING = 1, CREATED = 0, FINISHED = 2, CANCELED = 3;
        std::function<void()> task;
        int status = CREATED;
        int priority = 0;

        bool operator>=(CONST ThreadTask& __y) const
        {
            return priority >= __y.priority;
        }

    };

    class ThreadPoolImpl;

    class ThreadPool final : public virtual PointerImpl<ThreadPoolImpl>
    {
    public:
        explicit ThreadPool(const ThreadPoolData& config);

        DEFAULT_DESTRUCTION(ThreadPool);

        DELETE_COPY_ASSIGNMENT(ThreadPool);
        DELETE_COPY_CONSTRUCTION(ThreadPool);
        /**
         * 停止线程池
         */
        void stop();

        /**
         * 启动线程池
         */
        void start();


        /**
         * 提交一个任务
         * @param function 任务
         * @param priority 优先级
         */
        void submit(std::function<void()> function, int priority = 0);

        /**
         *
         * @return 线程池是否正在运行
         */
        bool isRunning() CONST;
    };
} // namespace tbs::threads


#endif // THREADPOOL_H
