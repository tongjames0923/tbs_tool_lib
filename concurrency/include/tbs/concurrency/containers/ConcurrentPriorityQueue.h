//
// Created by abstergo on 24-11-16.
//

#ifndef CONCURRENTPRIORTYQUEUE_H
#define CONCURRENTPRIORTYQUEUE_H

#include <tbs/concurrency/adapters.h>
#include <tbs/concurrency/containers/ConcurrentContainer.h>
#include <tbs/concurrency/sync_point/SyncPoint.h>
#include <optional>
#include <queue>

namespace tbs::concurrency::containers
{

    /**
     * @brief 线程安全的优先队列模板类。
     *
     * 该类实现了一个支持并发访问的优先队列。它继承自 `ConcurrentContainer` 并使用 `SyncPoint` 进行同步。
     *
     * @tparam T 存储在队列中的元素类型。
     * @tparam CONTAINER 底层容器类型，默认为 `std::vector<T>`。
     * @tparam COMPARE 比较函数类型，默认为 `std::greater_equal<T>`。
     * @tparam LOCK 用于同步的锁类型，默认为 `::SharedMutexLockAdapter`。
     */
    template <typename T, typename CONTAINER = std::vector<T>, typename COMPARE = std::greater_equal<T>, typename LOCK = ::SharedMutexLockAdapter>
    class ConcurrentPriorityQueue : public virtual concurrency::containers::ConcurrentContainer<std::priority_queue<T, CONTAINER, COMPARE>, LOCK>
    {
    private:
        mutable sync_point::SyncPoint m_syncPoint; // 用于同步的 SyncPoint 对象
        using Base = ConcurrentContainer<std::priority_queue<T, CONTAINER, COMPARE>, LOCK>; // 基类别名
    public:
        /**
         * @brief 向队列中添加一个元素。
         *
         * @param val 要添加的元素。
         */
        void push(const T& val)
        {
            Base::writeAsAtomic(
                [&](auto& q)
                {
                    q.push(val); // 将元素添加到队列中
                    m_syncPoint.accumulateFlag(1); // 更新同步标志
                });
        }

        /**
         * @brief 从队列中移除一个元素。
         */
        void pop()
        {
            Base::writeAsAtomic(
                [&](auto& q)
                {
                    if (q.empty())
                    {
                        return; // 队列为空时直接返回
                    }
                    q.pop(); // 移除队列顶部元素
                    m_syncPoint.accumulateFlag(-1); // 更新同步标志
                });
        }

        /**
         * @brief 从队列中获取并移除一个元素，等待指定时间。
         *
         * @param timeout 等待超时时间。
         * @return 获取到的元素，如果超时则返回空。
         */
        std::optional<T> poll(CONST time_utils::ms& timeout)
        {
            std::optional<T> ret;
            m_syncPoint.wait_flag(1,
                                  timeout,
                                  [&](CONST sync_point::SyncPoint& s, bool a, bool b, bool c, CONST int& t)
                                  {
                                      if (c) // 旗帜被设置，说明有元素被添加或移除，唤醒等待线程
                                      {
                                          Base::writeAsAtomic(
                                              [&](auto& q)
                                              {
                                                  if (!q.empty())
                                                  {
                                                      ret = q.top(); // 获取队列顶部元素
                                                      q.pop(); // 移除队列顶部元素
                                                  }
                                              });
                                      }
                                  });
            if (ret.has_value())
            {
                m_syncPoint.accumulateFlag(-1); // 更新同步标志
            }
            return ret;
        }


        /**
         * @brief 获取并移除队列顶部的元素。
         *
         * @return 队列顶部的元素，如果队列为空则返回
         */
        T poll()
        {
            std::optional<T> ret;
            do
            {
                ret = std::move(poll(time_utils::ms(2000)));
            }
            while (!ret.has_value());
            return ret.value();
        }


        /**
         * @brief 获取队列顶部的元素（常量版本）。
         *
         * @return 队列顶部的元素引用。
         */
        CONST T& top() CONST
        {
            CONST T* ret = nullptr;
            m_syncPoint.wait_flag(1,
                                  [&](CONST sync_point::SyncPoint& s, bool a, bool b, bool c, CONST int& t)
                                  {
                                      readAsAtomic(
                                          [&](auto& q)
                                          {
                                              ret = &q.top(); // 获取队列顶部元素的指针
                                          });
                                  });
            return *ret; // 返回队列顶部元素的引用
        }

        /**
         * @brief 获取队列顶部的元素。
         *
         * @return 队列顶部的元素。
         */
        T top()
        {
            std::optional<T> ret;
            m_syncPoint.wait_flag(1,
                                  [&](CONST sync_point::SyncPoint& s, bool a, bool b, bool c, CONST int& t)
                                  {
                                      Base::readAsAtomic(
                                          [&](auto& q)
                                          {
                                              ret = q.top(); // 获取队列顶部元素
                                          });
                                  });
            return ret.value(); // 返回队列顶部元素
        }

        /**
         * @brief 判断队列是否为空。
         *
         * @return 如果队列为空则返回 true，否则返回 false。
         */
        bool empty() const
        {
            return size() == 0; // 调用 size 方法判断队列是否为空
        }

        /**
         * @brief 获取队列的大小。
         *
         * @return 队列中元素的数量。
         */
        size_t size() const
        {
            size_t r = 0;
            Base::readAsAtomic(
                    [&r](auto &q)
                    {
                        r = q.size(); // 获取队列的大小
                    });
            return r; // 返回队列的大小
        }

        /**
         * @brief 清空队列。
         */
        void clear()
        {
            Base::writeAsAtomic(
                    [&](auto &q)
                    {
                        q.clear();           // 清空队列
                        m_syncPoint.reset(); // 重置同步标志
                    });
        }
};

}; // namespace tbs::concurrency::containers

#endif // CONCURRENTPRIORTYQUEUE_H