#include <concurrency/sync_point/SyncPoint.h>
#include <cstddef>

namespace tbs::concurrency::sync_point
{

SyncPoint::SyncPoint(CONST size_t &wait_count) : _impl(wait_count)
{
    init();
}

void SyncPoint::wait(
        unsigned long long ms, __predic_functional predic, CONST bool &flagCheck, CONST int &target,
        CONST bool &timeLimited, __on_predic_moment m)
{
    int i = _lock_mutex();
    if (timeLimited)
    {
        bool                         pred = false, flag_c = false;
        std::unique_lock<std::mutex> lock(_impl._mutexs[i]);
        bool                         r = _impl._conditions[i].wait_for(
                lock, time_utils::ms(ms),
                [&]()
                {
                    pred   = predic();
                    flag_c = flagCheck && getFlag() >= target;
                    return pred || flag_c;
                });
        if (m != nullptr)
        {
            m(*this, !r, pred, flag_c, target);
        }
    }
    else
    {
        bool                         pred = false, flag_c = false;
        std::unique_lock<std::mutex> lock(_impl._mutexs[i]);
        _impl._conditions[i].wait(
                lock,
                [&]()
                {
                    pred   = predic();
                    flag_c = flagCheck && getFlag() >= target;
                    return pred || flag_c;
                });
        if (m != nullptr)
        {
            m(*this, false, pred, flagCheck, target);
        }
    }
    {
        std::unique_lock<std::mutex> threadLock(_impl._mutexs[_impl._wait_count]);
        _impl._waiting_mutexs.push(i);
    }
    _impl._conditions[_impl._wait_count].notify_one();
}

void SyncPoint::init()
{
    for (size_t i = 0; i < _impl._wait_count; ++i)
    {
        _impl._waiting_mutexs.push(i);
    }
}

int SyncPoint::_lock_mutex()
{
    _impl._conditions[_impl._wait_count].notify_one();
    std::unique_lock<std::mutex> lock(_impl._mutexs[_impl._wait_count]);
    _impl._conditions[_impl._wait_count].wait(lock, [&]() { return !_impl._waiting_mutexs.empty(); });
    int r = _impl._waiting_mutexs.front();
    _impl._waiting_mutexs.pop();
    return r;
}
} // namespace tbs::concurrency::sync_point