//
// Created by abstergo on 24-11-15.
//

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <concurrency/adapters.h>
#include <concurrency/containers/ConcurrentContainer.h>
#include <concurrency/sync_point/SyncPoint.h>
#include <queue>

// 命名空间tbs::concurrency::containers用于定义并发容器
namespace tbs::concurrency::containers
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
class ConcurrentQueue : public virtual container::ConcurrentContainer<std::queue<T>, LockType>
{
    private:
        mutable sync_point::SyncPoint m_sync_point{1};
    public: /**
             * 获取队列中元素的数量
             *
             * 此函数通过原子操作读取队列的大小，确保在多线程环境下的安全性
             *
             * @return 队列中元素的数量
             */
        size_t size() const
        {
            size_t _s = 0;
            this->readAsAtomic([&](auto &q) { _s = q.size(); });
            return _s;
        }

        /**
         * 检查队列是否为空
         *
         * 利用size()函数的结果来判断队列是否为空，同样确保了多线程安全
         *
         * @return 如果队列为空，则返回true；否则返回false
         */
        bool empty() const
        {
            return size() == 0;
        }

        /**
         * 清空队列中的所有元素
         *
         * 此函数通过原子操作来清空队列，并重置同步点，确保操作的线程安全
         */
        void clear()
        {
            this->writeAsAtomic(
                    [&](auto &q)
                    {
                        q.clear();
                        m_sync_point.reset();
                    });
        }

        /**
         * 向队列中添加一个元素（拷贝构造）
         *
         * 使用原子操作来执行元素的添加，确保在多线程环境下安全地修改队列，并更新同步点
         *
         * @param item 要添加到队列中的元素
         */
        void push(const T &item)
        {
            this->writeAsAtomic(
                    [&](auto &q)
                    {
                        q.push(item);
                        m_sync_point.accumulateFlag(1);
                    });
        }

        /**
         * 向队列中添加一个元素（移动构造）
         *
         * 使用原子操作来执行元素的添加，适合于移动语义的元素，确保在多线程环境下安全地修改队列，并更新同步点
         *
         * @param item 要添加到队列中的元素，将被移动
         */
        void push(T &&item)
        {
            this->writeAsAtomic(
                    [&](auto &q)
                    {
                        q.push(std::move(item));
                        m_sync_point.accumulateFlag(1);
                    });
        }

        /**
         * 从队列中移除一个元素
         *
         * 此函数通过原子操作来执行元素的移除，确保在多线程环境下安全地修改队列，并更新同步点
         */
        void pop()
        {
            this->writeAsAtomic(
                    [&](auto &q)
                    {
                        if (q.empty())
                            return;
                        q.pop();
                        m_sync_point.accumulateFlag(-1);
                    });
        }

        /**
         * 从队列中获取并移除一个元素
         *
         * 此函数在确保线程安全的前提下，从队列中获取并移除一个元素，涉及到同步点的等待和更新
         *
         * @return 被移除的元素
         */
        T poll()
        {
            T item;
            m_sync_point.wait_flag(
                    1,
                    [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c, CONST int &t)
                    {
                        this->writeAsAtomic(
                                [&](auto &q)
                                {
                                    item = std::move(q.front());
                                    q.pop();
                                });
                    });
            m_sync_point.accumulateFlag(-1);
            return item;
        }

        /**
         * 获取队列中的第一个元素，不移除
         *
         * 此函数通过原子操作和同步点机制来安全地访问队列的第一个元素，确保多线程安全
         *
         * @return 队列中的第一个元素的引用
         */
        CONST T &front() CONST
        {
            CONST T *item = nullptr;
            m_sync_point.wait_flag(
                    1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c, CONST int &t)
                    { this->readAsAtomic([&](auto &q) { item = &q.back(); }); });
            return *item;
        }

        /**
         * 获取队列中的最后一个元素，不移除
         *
         * 此函数通过原子操作和同步点机制来安全地访问队列的最后一个元素，确保多线程安全
         *
         * @return 队列中的最后一个元素的引用
         */
        CONST T &back() CONST
        {
            CONST T *item = nullptr;
            m_sync_point.wait_flag(
                    1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c, CONST int &t)
                    { this->readAsAtomic([&](auto &q) { item = &q.back(); }); });
            return *item;
        }
};

}; // namespace tbs::concurrency::containers

#endif // CONCURRENTQUEUE_H