//
// Created by abstergo on 24-11-15.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H

#include <atomic>
#include <time_utils.hpp>
#include <defs.h>
#include <functional>
#include <condition_variable>
#include <utility>
#include <queue>

/**
 * 命名空间 tbs::concurrency::sync_point 用于同步点机制，提供线程同步的工具类。
 */
namespace tbs::concurrency::sync_point
{
/**
 * 类型别名 __predic_function 表示旧式的谓词函数指针类型。
 */
using __predic_function = bool (*)();

/**
 * 类型别名 __predic_functional 表示现代 C++ 风格的谓词函数类型。
 */
using __predic_functional = std::function<bool()>;

/**
 * SyncPoint 类用于线程同步，提供多种等待条件和标志的机制。
 *
 * 模板参数：
 * - SAME_TIME_WAIT_COUNT: 同时等待的线程数，默认为 1。
 */
template<size_t SAME_TIME_WAIT_COUNT = 1>
class SyncPoint
{
    private:
        /**
         * 原子整型 _flag 用于在不同线程间共享状态。
         */
        std::atomic_int _flag{0};

        /**
         * 互斥锁 _mutexs 用于保护共享资源，避免数据竞争。
         */
        std::mutex _mutexs[SAME_TIME_WAIT_COUNT + 1];

        /**
         * 条件变量 _conditions 用于线程间的通信，提高同步效率。
         */
        std::condition_variable _conditions[SAME_TIME_WAIT_COUNT + 1];

        /**
         * _thread_safe_mutex 和 _threadSafe_condition 用于管理等待队列。
         */
        std::mutex &             _thread_safe_mutex    = _mutexs[SAME_TIME_WAIT_COUNT];
        std::condition_variable &_threadSafe_condition = _conditions[SAME_TIME_WAIT_COUNT];

        /**
         * 静态断言确保 SAME_TIME_WAIT_COUNT 大于 0。
         */
        static_assert(SAME_TIME_WAIT_COUNT > 0, "SAME_TIME_WAIT_COUNT must be greater than 0");

        /**
         * _waiting_mutexs 用于管理等待的互斥锁索引。
         */
        std::queue<size_t> _waiting_mutexs;

        /**
         * 初始化等待队列。
         */
        inline void init()
        {
            for (size_t i = 0; i < SAME_TIME_WAIT_COUNT; ++i)
            {
                _waiting_mutexs.push(i);
            }
        }

        /**
         * 锁定互斥锁并从等待队列中获取一个可用的互斥锁索引。
         * @return 可用的互斥锁索引。
         */
        int _lock_mutex()
        {
            std::unique_lock<std::mutex> lock(_thread_safe_mutex);
            _threadSafe_condition.wait(
                    lock, [&]()
                    {
                        return !_waiting_mutexs.empty();
                    });
            int r = _waiting_mutexs.front();
            _waiting_mutexs.pop();
            return r;
        }

        /**
         * 私有方法 wait 提供核心的等待逻辑，支持超时和非超时等待。
         * @param ms 超时时间（毫秒）。
         * @param predic 谓词函数，决定等待条件。
         * @param flagCheck 是否检查内部标志。
         * @param target 目标标志值。
         * @param timeLimited 是否有限制时间。
         */
        void wait(
                unsigned long long ms, __predic_functional predic, CONST bool &flagCheck, CONST int &target = -1,
                CONST bool &       timeLimited                                                              = true)
        {
            int                          i = _lock_mutex();
            std::unique_lock<std::mutex> lock(_mutexs[i]);
            if (timeLimited)
            {
                _conditions[i].wait_for(
                        lock, time_utils::ms(ms), [&]()
                        {
                            return predic() || (flagCheck && getFlag() >= target);
                        });
            }
            else
            {
                _conditions[i].wait(
                        lock, [&]()
                        {
                            return predic() || (flagCheck && getFlag() >= target);
                        });
            } {
                std::unique_lock<std::mutex> threadLock(_thread_safe_mutex);
                _waiting_mutexs.push(i);
            }
            _threadSafe_condition.notify_one();
        }
    public:
        /**
         * 构造函数，初始化等待队列。
         */
        SyncPoint()
        {
            init();
        }

        /**
         *  等待直到指定的现代谓词函数返回 true。
         * @param f
         */
        void wait_to_predicate(__predic_function f)
        {
            wait(
                    0, [f]()
                    {
                        return f();
                    }, false, -1, false);
        }

        /**
         *  等待直到指定的现代谓词函数返回 true。
         * @param f
         */
        void wait_to_predicate(__predic_functional f)
        {
            wait(0, std::move(f), false, -1, false);
        }

        /**
      * 等待直到指定的时间。
      * @param to 超时时间。
      */
        void wait_util(CONST time_utils::ms &to)
        {
            wait_util(
                    to, [&]()
                    {
                        return false;
                    });
        }

        /**
         * 等待直到指定的谓词函数返回 true。
         * @param to 超时时间。
         * @param f 谓词函数。
         */
        void wait_util(CONST time_utils::ms &to, __predic_function f)
        {
            wait_util(
                    to, [&]()
                    {
                        return f();
                    });
        }

        /**
         * 等待直到指定的现代谓词函数返回 true。
         * @param timeout 超时时间。
         * @param f 现代谓词函数。
         */
        void wait_util(CONST time_utils::ms &timeout, __predic_functional f)
        {
            wait(timeout.count(), std::move(f), false);
        }

        /**
         * 重置内部标志为 0。
         */
        void reset()
        {
            _flag = 0;
        }

        /**
         * 等待直到内部标志达到目标值。
         * @param target 目标标志值。
         */
        void wait_flag(CONST int &target)
        {
            wait_flag(
                    target, [&]()
                    {
                        return false;
                    });
        }

        /**
         * 等待直到内部标志达到目标值或谓词函数返回 true。
         * @param target 目标标志值。
         * @param f 谓词函数。
         */
        void wait_flag(CONST int &target, __predic_function f)
        {
            wait_flag(
                    target, [&]()
                    {
                        return f();
                    });
        }

        /**
         * 等待直到内部标志达到目标值或现代谓词函数返回 true。
         * @param target 目标标志值。
         * @param f 现代谓词函数。
         */
        void wait_flag(CONST int &target, __predic_functional f)
        {
            wait(0, std::move(f), true, target, false);
        }

        /**
         * 等待直到内部标志达到目标值或超时。
         * @param target 目标标志值。
         * @param m 超时时间。
         */
        void wait_flag(CONST int &target, CONST time_utils::ms &m)
        {
            wait_flag(
                    target, m, [&]()
                    {
                        return false;
                    });
        }

        /**
         * 等待直到内部标志达到目标值、超时或谓词函数返回 true。
         * @param target 目标标志值。
         * @param ms 超时时间。
         * @param f 谓词函数。
         */
        void wait_flag(CONST int &target, CONST time_utils::ms &ms, __predic_function f)
        {
            wait_flag(
                    target, ms, [&]()
                    {
                        return f();
                    });
        }

        /**
         * 等待直到内部标志达到目标值、超时或现代谓词函数返回 true。
         * @param target 目标标志值。
         * @param ms 超时时间。
         * @param f 现代谓词函数。
         */
        void wait_flag(CONST int &target, CONST time_utils::ms &ms, __predic_functional f)
        {
            wait(ms.count(), std::move(f), true, target);
        }

        /**
         * 增加内部标志的值并通知一个等待的线程。
         * @param delta 增加的值。
         * @return 增加后的标志值。
         */
        int accumulateFlag(const int &delta)
        {
            const int k = _flag += delta;
            wakeup();
            return k;
        }

        /**
         * 获取当前的内部标志值。
         * @return 当前的标志值。
         */
        int getFlag() const
        {
            return _flag;
        }

        /**
         * 唤醒所有等待的线程。防止等待睡死
         */
        void wakeup()
        {
            for (int i = 0; i < SAME_TIME_WAIT_COUNT; i++)
            {
                _conditions[i].notify_one();
            }
        }
};
}

#endif // TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H