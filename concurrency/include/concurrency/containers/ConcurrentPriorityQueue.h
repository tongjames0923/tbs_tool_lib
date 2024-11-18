//
// Created by abstergo on 24-11-16.
//

#ifndef CONCURRENTPRIORTYQUEUE_H
#define CONCURRENTPRIORTYQUEUE_H
#include <queue>
#include <concurrency/LockAdapter.h>
#include <concurrency/sync_point/SyncPoint.h>

#include <concurrency/adapters.h>

namespace tbs::concurrency::containers
{

template<typename T, typename CONTAINER=std::vector<T>, typename COMPARE=std::greater_equal<T>, typename LOCK=
         SharedMutexLockAdapter>
class ConcurrentPriorityQueue
{
    private:
        std::priority_queue<T, CONTAINER, COMPARE> queue;
        tbs::concurrency::sync_point::SyncPoint    syncPoint;
        mutable LOCK                               lock;
        using _lock_guard = guard::auto_op_lock_guard<LOCK>; // 定义锁的智能指针类型

        /**
     * @brief 静态常量，用于判断是否使用了共享锁
     */
        constexpr static bool __is_shared_lock =
                std::is_base_of_v<SharedMutexLockAdapter, LOCK> || std::is_base_of_v<
                    SharedTimedMutexLockAdapter, LOCK>;

        #if __is_shared_lock==true
    using _shared_lock_guard = tbs::concurrency::guard::auto_shared_lock_op_guard<LockType>;
        #endif
    public:
        void push(T &&item)
        {
            {
                _lock_guard lock_guard(lock);
                queue.push(item);
                syncPoint.accumulateFlag(1);
            }

        }

        void push(const T &item)
        {
            {
                _lock_guard lock_guard(lock);
                queue.push(item);
                syncPoint.accumulateFlag(1);
            }

        }

        T pop()
        {
            T result;
            syncPoint.wait_flag(
                    1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c,CONST int &t)
                    {
                        _lock_guard l(lock);
                        result = std::move(queue.top());
                        queue.pop();
                        syncPoint.accumulateFlag(-1);
                    });

            return result;
        }

        T &top()
        {
            T *ptr = nullptr;
            syncPoint.wait_flag(
                    1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c,CONST int &t)
                    {
                        #if __is_shared_lock==true
                        _shared_lock_guard l(lock);
                        #else
                        _lock_guard l(lock);
                        #endif
                        ptr = &queue.top();
                    });
            return *ptr;
        }

        CONST T &top() const
        {
            T *ptr = nullptr;
            syncPoint.wait_flag(
                    1, [&](CONST sync_point::SyncPoint &s, bool a, bool b, bool c,CONST int &t)
                    {
                        #if __is_shared_lock==true
                        _shared_lock_guard l(lock);
                        #else
                        _lock_guard l(lock);
                        #endif
                        ptr = &queue.top();
                    });
            return *ptr;
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