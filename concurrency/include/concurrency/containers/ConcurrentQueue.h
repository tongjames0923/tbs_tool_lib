//
// Created by abstergo on 24-11-15.
//

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <queue>
#include <concurrency/containers/ConcurrentContainer.h>
#include <concurrency/adapters.h>
#include <concurrency/sync_point/SyncPoint.h>



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
    public:
        size_t size() const
        {
            size_t _s = 0;
            this->readAsAtomic(
                    [&](auto &q)
                    {
                        _s = q.size();
                    });
            return _s;
        }

        bool empty() const
        {
            return size() == 0;
        }

        void push(const T &item)
        {
            this->writeAsAtomic(
                    [&](auto &q)
                    {
                        q.push(item);
                        m_sync_point.accumulateFlag(1);
                    });
        }

        void push(T &&item)
        {
            this->writeAsAtomic(
                    [&](auto &q)
                    {
                        q.push(std::move(item));
                        m_sync_point.accumulateFlag(1);
                    });
        }

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

        T poll()
        {
            T item;
            m_sync_point.wait_flag(
                    1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c, CONST int &t)
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

        CONST T &front() CONST
        {
            CONST T *item = nullptr;
            m_sync_point.wait_flag(
                    1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c, CONST int &t)
                    {
                        this->readAsAtomic(
                                [&](auto &q)
                                {
                                    item = &q.back();
                                });
                    });
            return *item;
        }

        CONST T &back() CONST
        {
            CONST T *item = nullptr;
            this->readAsAtomic(
                    [&](auto &q)
                    {
                        m_sync_point.wait_flag(
                                1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c, CONST int &t)
                                {
                                    item = &q.back();
                                });
                    });
            return *item;
        }
};

};

#endif //CONCURRENTQUEUE_H