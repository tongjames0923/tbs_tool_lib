//
// Created by abstergo on 24-11-18.
//

#ifndef CONCURRENTCONTAINER_H
#define CONCURRENTCONTAINER_H

#include <tbs/concurrency/adapters.h>
#include <tbs/concurrency/sync_point/SyncPoint.h>

/**
 * @brief 命名空间 tbs::concurrency::container 包含并发容器类
 */
namespace tbs::concurrency::containers
{

/**
 * @brief ConcurrentContainer 类提供了一个线程安全的容器封装
 *
 * 该类模板允许用户在多线程环境中安全地读写容器。它通过使用锁来同步访问，
 * 并提供了读写操作的原子性保证。可以使用不同的锁适配器来适应不同的并发需求。
 *
 * @tparam CONTAINER 容器类型，例如std::vector、std::list等
 * @tparam LOCK_ADAPTER 锁适配器类型，用于同步访问
 */
template<typename CONTAINER, typename LOCK_ADAPTER>
class ConcurrentContainer
{
    private:
        CONTAINER            m_container;                            // 存储容器
        mutable LOCK_ADAPTER m_lock;                                 // 用于同步的锁
        using _lock_guard = guard::auto_op_lock_guard<LOCK_ADAPTER>; // 定义锁的智能指针类型

        /**
         * @brief 静态常量，用于判断是否使用了共享锁
         */
        constexpr static bool __is_shared_lock = std::is_base_of_v<SharedMutexLockAdapter, LOCK_ADAPTER> ||
                                                 std::is_base_of_v<SharedTimedMutexLockAdapter, LOCK_ADAPTER>;

        using _read_Func  = std::function<void(CONST CONTAINER &)>;
        using _write_Func = std::function<void(CONTAINER &)>;
    protected:
        /**
         * @brief 以原子性方式读取容器
         *
         * 该方法确保在读取操作期间对容器的访问是线程安全的。根据锁的类型，
         * 它会选择性地使用共享锁来允许多个读取者同时访问容器。
         *
         * @param f 一个函数对象，用于执行读取操作
         */
        void readAsAtomic(_read_Func f) CONST
        {
            if constexpr (__is_shared_lock)
            {
                using _shared_lock_guard = guard::auto_shared_lock_op_guard<LOCK_ADAPTER>;
                _shared_lock_guard g(m_lock);
            }
            else
            {
                _lock_guard g(m_lock);
            }
            f(m_container);
        }

        /**
         * @brief 以原子性方式写入容器
         *
         * 该方法确保在写入操作期间对容器的访问是线程安全的。它总是使用独占锁来保证
         * 在任何时刻只有一个线程可以写入容器。
         *
         * @param f 一个函数对象，用于执行写入操作
         */
        void writeAsAtomic(_write_Func f)
        {
            _lock_guard g(m_lock);
            f(m_container);
        }
    public:
        virtual ~ConcurrentContainer() = default;

        /**
         * @brief 移动构造函数
         */
        ConcurrentContainer(ConcurrentContainer &&o) noexcept
        {
            m_container = std::move(o.m_container);
        }

        /**
         * @brief 移动赋值运算符
         */
        ConcurrentContainer &operator=(ConcurrentContainer &&o) noexcept
        {
            m_container = std::move(o.m_container);
        }

        /**
         * @brief 拷贝构造函数
         */
        ConcurrentContainer(CONST ConcurrentContainer &o)
        {
            m_container = o.m_container;
        }

        /**
         * @brief 拷贝赋值运算符
         */
        ConcurrentContainer &operator=(CONST ConcurrentContainer &o)
        {
            m_container = o.m_container;
        }

        /**
         * @brief 默认构造函数
         */
        ConcurrentContainer() = default;
};
} // namespace tbs::concurrency::container

#endif // CONCURRENTCONTAINER_H
