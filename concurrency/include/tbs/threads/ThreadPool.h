//
// Created by abstergo on 24-11-21.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <functional>
#include <tbs/PointerToImpl.h>
namespace tbs::threads
{

    constexpr static int EXCEPTION_TASK_COUNT_FULL = -1; // 任务队列已满异常
    constexpr static int EXCEPTION_TASK_ERROR = -2; // 任务执行错误异常

    class ThreadPool;
    struct ThreadPoolData;
    struct error_info;
    struct ThreadTask;
    struct event_info;

    /**
     * 异常处理器类型定义
     * @param e 错误信息指针
     * @param config 线程池配置指针
     * @param task 出错的任务指针
     * @param pool 线程池指针
     */
    using exception_handler = std::function<void(error_info* e, ThreadPoolData* config, ThreadTask* task, ThreadPool* pool)>;

    /**
     * 线程池事件处理器类型定义
     * @param info 事件信息指针
     * @param config 线程池配置指针
     * @param pool 线程池指针
     */
    using thread_pool_event_handler = std::function<void(event_info* info, ThreadPoolData* config, ThreadPool* pool)>;

    struct event_info
    {
        constexpr static int WAITTING = 0; // 等待状态
        constexpr static int PICKED = 1; // 被选中状态
        constexpr static int RUNNING = 2; // 运行状态
        constexpr static int FINISHED = 3; // 完成状态
        constexpr static int CANCELED = -1; // 取消状态

        int signal; // 事件信号
        ThreadTask* runningTask; // 当前运行的任务
        size_t current_thread; // 当前线程索引
        size_t total_thread; // 总线程数
        size_t waitting_tasks; // 等待中的任务数
    };

    struct error_info
    {
        int signal; // 错误信号
        std::exception* exception; // 异常对象指针
        size_t threadIndex; // 发生异常的线程索引
    };

    struct ThreadPoolData
    {
        const char* threadPoolName; // 线程池名称
        size_t threadCount; // 线程池中的线程数
        size_t maxTaskCount; // 最大任务数
        bool running; // 线程池是否正在运行
        exception_handler exceptionHandler = nullptr; // 异常处理器
        thread_pool_event_handler eventHandler = nullptr; // 事件处理器
        size_t maxIdleThreadCount = threadCount; // 最大空闲线程数
        size_t maxIdleTime = 5000; // 空闲线程的最大空闲时间（毫秒）
    };

    struct ThreadTask
    {
        constexpr static int RUNNING = 1; // 运行状态
        constexpr static int CREATED = 0; // 创建状态
        constexpr static int FINISHED = 2; // 完成状态
        constexpr static int CANCELED = 3; // 取消状态

        std::function<void()> task; // 任务函数
        int status = CREATED; // 任务状态
        int priority = 0; // 任务优先级

        bool operator>=(const ThreadTask& __y) const
        {
            return priority >= __y.priority; // 比较任务优先级
        }
    };

    class ThreadPoolImpl;

    /**
     * 线程池类
     * @note 线程池不支持拷贝，只支持移动
     * @note 线程池不支持继承
     */
    class ThreadPool final : protected virtual PointerImpl<ThreadPoolImpl>
    {
    public:
        /**
         * 构造函数
         * @param threadPoolName 线程池名称
         * @param threadCount 线程池中的线程数
         * @param maxTaskCount 最大任务数
         * @param maxIdleThreadCount 最大空闲线程数
         * @param maxIdleTime 空闲线程的最大空闲时间（毫秒）
         * @param exceptionHandler 异常处理器
         * @param eventHandler 事件处理器
         */
        explicit ThreadPool(const char* threadPoolName = "default",
                            size_t threadCount = 2,
                            size_t maxTaskCount = 32,
                            size_t maxIdleThreadCount = 2,
                            size_t maxIdleTime = 5000,
                            exception_handler exceptionHandler = nullptr,
                            thread_pool_event_handler eventHandler = nullptr);

        DEFAULT_DESTRUCTION(ThreadPool); // 默认析构函数

        DELETE_COPY_ASSIGNMENT(ThreadPool); // 删除拷贝赋值操作
        DELETE_COPY_CONSTRUCTION(ThreadPool); // 删除拷贝构造函数

        DEFAULT_MOVE_ASSIGNMENT(ThreadPool); // 默认移动赋值操作
        DEFAULT_MOVE_CONSTRUCTION(ThreadPool); // 默认移动构造函数

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
         * @param function 任务函数
         * @param priority 任务优先级
         */
        void submit(std::function<void()> function, int priority = 0);

        /**
         * 获取线程池是否正在运行
         * @return 线程池是否正在运行
         */
        bool isRunning() const;

        /**
         * 获取线程池配置（常量版本）
         * @return 线程池配置
         */
        const ThreadPoolData& getConfig() const;

        /**
         * 获取线程池配置（可修改版本）
         * @return 线程池配置
         */
        ThreadPoolData& getConfig();
    };
} // namespace tbs::threads


#endif // THREADPOOL_H
