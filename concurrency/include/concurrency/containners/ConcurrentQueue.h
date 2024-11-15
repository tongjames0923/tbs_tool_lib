//
// Created by abstergo on 24-11-15.
//

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <queue>
#include <concurrency/adapters.h>
#include <concurrency/sync_point/SyncPoint.h>



// 命名空间tbs::concurrency::containners用于定义并发容器
namespace tbs::concurrency::containners
{

/**
 * @brief 并发队列类
 *
 * 该类模板提供了一个线程安全的队列实现。通过使用锁和同步点机制来保证线程安全。
 *
 * @tparam T 队列中元素的类型
 * @tparam LockType 锁的类型，用于保护队列的并发访问
 */
template<typename T, typename LockType>
class ConcurrentQueue
{
    private:
        std::queue<T> m_queue;                                                     // 存储元素的队列
        LockType      m_lock;                                                      // 用于同步访问的锁
        using _lock_guard = tbs::concurrency::guard::auto_op_lock_guard<LockType>; // 定义锁的智能指针类型
        mutable tbs::concurrency::sync_point::SyncPoint<1> m_sync_point;           // 同步点，用于在队列操作中同步线程

    public:
        /**
         * @brief 构造函数，通过移动方式初始化队列
         *
         * @param q 一个右值引用的队列，其内容将被移动到新创建的ConcurrentQueue中
         */
        explicit ConcurrentQueue(std::queue<T> &&q)
        : m_queue(std::move(q))
        {
            m_sync_point.reset();
            m_sync_point.accumulateFlag(q.size());
        }

        ConcurrentQueue(ConcurrentQueue &&q) noexcept
        : m_queue(std::move(q.m_queue))
        {
            m_queue = std::move(q.m_queue);
            m_sync_point.reset();
            m_sync_point.accumulateFlag(q.size());
            q.m_sync_point.reset();
        }

        ConcurrentQueue &operator=(ConcurrentQueue &&q) noexcept
        {
            m_queue = std::move(q.m_queue);
            m_sync_point.reset();
            m_sync_point.accumulateFlag(q.size());
            q.m_sync_point.reset();
            return *this;
        }

        ConcurrentQueue(CONST ConcurrentQueue &q)
        {
            m_queue = q.m_queue;
            m_sync_point.reset();
            m_sync_point.accumulateFlag(q.size());
        }

        ConcurrentQueue &operator=(CONST ConcurrentQueue &q)
        {
            m_queue = q.m_queue;
            m_sync_point.reset();
            m_sync_point.accumulateFlag(q.size());
            return *this;
        }

        /**
         * @brief 构造函数，通过拷贝方式初始化队列
         *
         * @param q 一个常量引用的队列，其内容将被拷贝到新创建的ConcurrentQueue中
         */
        explicit ConcurrentQueue(const std::queue<T> &q)
        : m_queue(q)
        {
            m_sync_point.reset();
            m_sync_point.accumulateFlag(q.size());
        }

        /**
         * @brief 默认构造函数，创建一个空的并发队列
         */
        explicit ConcurrentQueue()
        {
            m_sync_point.reset();
            m_sync_point.accumulateFlag(0);
        }

        /**
         * @brief 向队列中添加元素，通过移动方式
         *
         * @param t 要添加到队列中的元素，通过右值引用传递
         */
        void push(T &&t)
        {
            _lock_guard l(m_lock);
            m_queue.push(std::move(t));
            m_sync_point.accumulateFlag(1);
        }

        /**
         * @brief 向队列中添加元素，通过拷贝方式
         *
         * @param t 要添加到队列中的元素，通过常量引用传递
         */
        void push(const T &t)
        {
            _lock_guard l(m_lock);
            m_queue.push(t);
            m_sync_point.accumulateFlag(1);
        }

        /**
         * @brief 从队列中移除并返回第一个元素
         *
         * @return T 移除的元素，通过值返回
         */
        T pop()
        {
            m_sync_point.wait_flag(1);
            _lock_guard l(m_lock);
            T           t = std::move(m_queue.front());
            m_queue.pop();
            m_sync_point.accumulateFlag(-1);
            return std::move(t);
        }

        /**
         * @brief 检查队列是否为空
         *
         * @return true 如果队列为空
         * @return false 如果队列不为空
         */
        bool empty()CONST
        {
            return size() == 0;
        }

        /**
         * @brief 获取队列中元素的数量
         *
         * @return size_t 队列中元素的数量
         */
        size_t size()CONST
        {
            return m_sync_point.getFlag();
        }

        /**
         * @brief 获取队列的第一个元素，不移除它
         *
         * @return T& 第一个元素的引用
         */
        T &front()
        {
            m_sync_point.wait_flag(1);
            _lock_guard l(m_lock);
            return m_queue.front();
        }

        /**
         * @brief 获取队列的最后一个元素，不移除它
         *
         * @return T& 最后一个元素的引用
         */
        T &back()
        {
            m_sync_point.wait_flag(1);
            _lock_guard l(m_lock);
            return m_queue.back();
        }

        /**
         * @brief 获取队列的第一个元素，不移除它（常量版本）
         *
         * @return CONST T& 第一个元素的常量引用
         */
        CONST T &front() const
        {
            m_sync_point.wait_flag(1);
            _lock_guard l(m_lock);
            return m_queue.front();
        }

        /**
         * @brief 获取队列的最后一个元素，不移除它（常量版本）
         *
         * @return CONST T& 最后一个元素的常量引用
         */
        CONST T &back() const
        {
            m_sync_point.wait_flag(1);
            _lock_guard l(m_lock);
            return m_queue.back();
        }

        /**
         * @brief 清空队列中的所有元素
         */
        void clear()
        {
            _lock_guard l(m_lock);
            m_queue.clear();
            m_sync_point.reset();
        }
};

};

#endif //CONCURRENTQUEUE_H