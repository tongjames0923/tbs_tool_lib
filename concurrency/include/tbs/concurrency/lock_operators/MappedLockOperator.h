//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H

#include <condition_variable>
#include <mutex>
#include <tbs/concurrency/LockAdapter.h>
#include <tbs/time_utils.hpp>
#include <thread>
#include <unordered_map>

// 定义一个模板类MappedLockOperator，用于对类型为T的对象进行锁操作
// 继承自tbs::concurrency::Lockable<T>，提供锁的抽象接口
template<typename T>
class MappedLockOperator : public virtual tbs::concurrency::Lockable<T>
{
    private:
        // 使用unordered_map来存储每个对象的锁信息
        std::unordered_map<T *, std::thread::id> m_locks;
        // 使用recursive_mutex来保证线程安全
        mutable std::recursive_mutex m_mutex;
    public:
        // 默认构造函数
        MappedLockOperator() = default;

        // 默认析构函数
        virtual ~MappedLockOperator() = default;

        // 检查当前线程是否持有指定对象的锁
        bool heldByCurrentThread(T &m) CONST override
        {
            std::lock_guard<std::recursive_mutex> g(m_mutex);
            if (m_locks.contains(&m))
            {
                return m_locks[&m] == std::this_thread::get_id();
            }
            return false;
        }

        // 检查指定对象是否被锁定，不考虑当前线程
        bool locked(T &m) CONST override
        {
            std::lock_guard<std::recursive_mutex> g(m_mutex);
            return m_locks.contains(&m);
        }

        // 尝试在指定时间内锁定对象
        bool try_lock(T &m, unsigned long long timeout) override
        {
            bool ret = false;
            // 如果T类型支持timed_mutex或recursive_timed_mutex，则直接使用try_lock_for方法
            if constexpr (std::is_base_of<std::timed_mutex, T>::value || std::is_base_of<
                              std::recursive_timed_mutex, T>::value)
            {
                ret = m.try_lock_for(time_utils::ms(timeout));
            }
            else
            {
                // 否则，使用condition_variable和mutex来实现等待锁
                std::condition_variable      cv;
                std::mutex                   mtx;
                std::unique_lock<std::mutex> ul(mtx);
                ret = cv.wait_for(
                        ul, time_utils::ms(timeout), [&]()
                        {
                            return m.try_lock();
                        });
            }
            // 如果成功锁定，则更新锁信息
            if (ret)
            {
                std::lock_guard<std::recursive_mutex> g(m_mutex);
                m_locks[&m] = std::this_thread::get_id();
            }
            return ret;
        }

        // 锁定对象，并更新锁信息
        void lock(T &m) override
        {
            m.lock();
            std::lock_guard<std::recursive_mutex> g(m_mutex);
            m_locks[&m] = std::this_thread::get_id();
        }

        // 解锁对象，并从锁信息中移除
        void unlock(T &m) override
        { {
                std::lock_guard<std::recursive_mutex> g(m_mutex);
                m_locks.erase(&m);
            }
            m.unlock();
        }

};

#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H