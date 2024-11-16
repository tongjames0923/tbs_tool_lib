//
// Created by abstergo on 24-11-16.
//

#ifndef CONCURRENTPRIORTYQUEUE_H
#define CONCURRENTPRIORTYQUEUE_H
#include <queue>
#include <concurrency/LockAdapter.h>
#include <concurrency/sync_point/SyncPoint.h>



namespace tbs::concurrency::containers
{

template<typename T, typename CONTAINER, typename COMPARE, typename LOCK>
class ConcurrentPriorityQueue
{
    private:
        std::priority_queue<T, CONTAINER, COMPARE> queue;
        tbs::concurrency::sync_point::SyncPoint    syncPoint;
        mutable LOCK                               lock;
        using _lock_guard = guard::auto_op_lock_guard<LOCK>; // 定义锁的智能指针类型
    public:
        void push(T &&item)
        { {
                _lock_guard lock_guard(lock);
                queue.push(item);
            }
            syncPoint.accumulateFlag(1);
        }

        void push(const T &item)
        { {
                _lock_guard lock_guard(lock);
                queue.push(item);
            }
            syncPoint.accumulateFlag(1);
        }

        T pop()
        {
            syncPoint.wait_flag(1);
            _lock_guard lock_guard(lock);
            T           obj = std::move(queue.top());
            queue.pop();
            syncPoint.accumulateFlag(-1);
            return obj;
        }

        T &top()
        {
            _lock_guard lock_guard(lock);
            return queue.top();
        }

        CONST T &top() const
        {
            _lock_guard lock_guard(lock);
            return queue.top();
        }

        bool empty() const
        {
            return size() == 0;
        }

        size_t size() const
        {
            return syncPoint.getFlag();
        }

        void clear()
        {
            _lock_guard lock_guard(lock);
            queue.clear();
            syncPoint.reset();
        }

};

};

#endif //CONCURRENTPRIORTYQUEUE_H