//
// Created by abstergo on 24-11-19.
//

#ifndef SYNCPOINTIMPLSTRUCT_H
#define SYNCPOINTIMPLSTRUCT_H
#include <atomic>
#include <mutex>
#include <queue>
#include <vector>

namespace tbs::concurrency::sync_point
{
class SyncPoint;
}
namespace tbs::concurrency::sync_point::__detail
{
    struct SyncPointImpl
    {
            private:
                friend class tbs::concurrency::sync_point::SyncPoint;
            /**
             * 原子整型 _flag 用于在不同线程间共享状态。
             */
            std::atomic_int _flag{0};
            size_t          _wait_count;

            /**
             * 互斥锁 _mutexs 用于保护共享资源，避免数据竞争。
             */
            std::vector<std::mutex> _mutexs;

            /**
             * 条件变量 _conditions 用于线程间的通信，提高同步效率。
             */
            std::vector<std::condition_variable> _conditions;

            /**
             * _waiting_mutexs 用于管理等待的互斥锁索引。
             */
            std::queue<size_t> _waiting_mutexs;
            explicit SyncPointImpl(const size_t &wait_count)
                : _wait_count{wait_count}, _mutexs{wait_count + 1}, _conditions{wait_count + 1}
            {}
    };
}

#endif //SYNCPOINTIMPLSTRUCT_H
