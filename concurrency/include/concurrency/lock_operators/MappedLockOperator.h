//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H

#include <concurrency/LockAdapter.h>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <time_utils.hpp>

/**
 * @brief 锁信息模板结构体
 *
 * 该结构体用于存储锁的相关信息，包括锁的拥有者、锁的状态以及被锁对象的指针
 * 通过模板参数T，使得该结构体可以应用于多种类型的对象
 *
 * @tparam T 被锁对象的类型
 */
template<typename T>
struct LockInfo
{
    // 存储锁的拥有者的线程ID，用于标识哪个线程持有该锁
    std::thread::id owner;

    // 标记锁的状态，true表示已锁定，false表示未锁定
    bool locked = false;

    // 指向被锁对象的指针，用于关联锁和具体对象
    T *object;
};


// 定义一个模板类MappedLockOperator，用于对类型为T的对象进行锁操作
// 继承自tbs::concurrency::Lockable<T>，提供锁的抽象接口
template<typename T>
class MappedLockOperator : public virtual tbs::concurrency::Lockable<T>
{
    private:
        // 使用unordered_map来存储每个对象的锁信息
        std::unordered_map<T *, LockInfo<T> > m_locks;
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
                auto item = m_locks.at(&m);
                return item.locked && item.owner == std::this_thread::get_id();
            }
            return false;
        }

        // 检查指定对象是否被锁定，不考虑当前线程
        bool locked(T &m) CONST override
        {
            std::lock_guard<std::recursive_mutex> g(m_mutex);
            if (m_locks.contains(&m))
                return m_locks.at(&m).locked;
            return false;
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
                m_locks.insert_or_assign(&m, LockInfo<T>{std::this_thread::get_id(), true, &m});
            }
            return ret;
        }

        // 锁定对象，并更新锁信息
        void lock(T &m) override
        {
            m.lock();
            std::lock_guard<std::recursive_mutex> g(m_mutex);
            m_locks.insert_or_assign(&m, LockInfo<T>{std::this_thread::get_id(), true, &m});
        }

        // 解锁对象，并从锁信息中移除
        void unlock(T &m) override
        {
            m.unlock();
            std::lock_guard<std::recursive_mutex> g(m_mutex);
            m_locks.erase(&m);
        }

};

#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H