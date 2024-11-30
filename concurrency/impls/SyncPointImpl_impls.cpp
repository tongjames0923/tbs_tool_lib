//
// Created by tongj on 24-11-21.
//


#ifndef SYNCPOINTIMPL_H
#define SYNCPOINTIMPL_H
#include <atomic>
#include <tbs/concurrency/sync_point/SyncPoint.h>
namespace tbs::concurrency::sync_point
{
    class SyncPointImpl
    {
    public:
        /**
         * 原子整型 _flag 用于在不同线程间共享状态。
         */
        std::atomic_int _flag{0};
        size_t _wait_count;

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

        explicit SyncPointImpl(const size_t& wait_count = 1) : _wait_count{wait_count}, _mutexs{wait_count + 1}, _conditions{wait_count + 1}
        {
        }

        void init()
        {
            for (size_t i = 0; i < _wait_count; ++i)
            {
                _waiting_mutexs.push(i);
            }
        }

        void timeLimitWait(const time_utils::ms& ms,
                           __predic_functional predic,
                           const bool& flagCheck,
                           const int& target,
                           const size_t& i,
                           bool& pred,
                           bool& flag_c,
                           bool& r,
                           std::unique_lock<std::mutex>& lock)
        {
            r = _conditions[i].wait_for(lock,
                                        time_utils::ms(ms),
                                        [&]()
                                        {
                                            pred = predic();
                                            flag_c = flagCheck && _flag >= target;
                                            return pred || flag_c;
                                        });
        }
        void predictWait(__predic_functional predic, const bool& flagCheck, const int& target, const size_t& i, bool& pred, bool& flag_c, std::unique_lock<std::mutex>& lock)
        {
            _conditions[i].wait(lock,
                                [&]()
                                {
                                    pred = predic();
                                    flag_c = flagCheck && _flag >= target;
                                    return pred || flag_c;
                                });
        }
        void wait_impl(const time_utils::ms& ms,
                       SyncPoint& sp,
                       __predic_functional predic,
                       const bool& flagCheck,
                       const int& target,
                       const bool& timeLimited,
                       __on_predic_moment m,
                       const size_t& i)
        {
            bool pred = false;
            bool flag_c = false;
            bool r = false;
            std::unique_lock<std::mutex> lock(_mutexs[i]);
            if (timeLimited)
            {
                timeLimitWait(ms, predic, flagCheck, target, i, pred, flag_c, r, lock);
            }
            else
            {
                predictWait(predic, flagCheck, target, i, pred, flag_c, lock);
            }
            if (m != nullptr)
            {
                m(sp, !r, pred, flag_c, target);
            }
        }
        void wait(CONST time_utils::ms& ms, SyncPoint& sp, __predic_functional predic, CONST bool& flagCheck, CONST int& target, CONST bool& timeLimited, __on_predic_moment m)
        {
            const size_t i = _lock_mutex();
            {
                wait_impl(ms, sp, predic, flagCheck, target, timeLimited, m, i);
            }
            {
                std::unique_lock<std::mutex> threadLock(_mutexs[_wait_count]);
                _waiting_mutexs.push(i);
            }
            _conditions[_wait_count].notify_one();
        }

        size_t _lock_mutex()
        {
            _conditions[_wait_count].notify_one();
            std::unique_lock<std::mutex> lock(_mutexs[_wait_count]);
            _conditions[_wait_count].wait(lock, [&]() { return !_waiting_mutexs.empty(); });
            const size_t r = _waiting_mutexs.front();
            _waiting_mutexs.pop();
            return r;
        }
    };
}

#endif //SYNCPOINTIMPL_H
