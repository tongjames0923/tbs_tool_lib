#include <concurrency/sync_point/SyncPoint.h>
#include <cstddef>



namespace tbs::concurrency::sync_point
{

SyncPoint::SyncPoint(CONST size_t &wait_count)
: _wait_count{wait_count}, _mutexs(wait_count + 1), _conditions(wait_count + 1)
{
    init();
}

void SyncPoint::wait(
        unsigned long long ms, __predic_functional         predic, CONST bool &flagCheck, CONST int &target,
        CONST bool &       timeLimited, __on_predic_moment m)
{
    int i = _lock_mutex();
    if (timeLimited)
    {
        bool                         pred = false, flag_c = false;
        std::unique_lock<std::mutex> lock(_mutexs[i]);
        bool                         r = _conditions[i].wait_for(
                lock, time_utils::ms(ms), [&]()
                {
                    pred   = predic();
                    flag_c = flagCheck && getFlag() >= target;
                    return pred || flag_c;
                });
        if (m != nullptr)
        {
            m(*this, !r, pred, flagCheck, target);
        }
    }
    else
    {
        bool                         pred = false, flag_c = false;
        std::unique_lock<std::mutex> lock(_mutexs[i]);
        _conditions[i].wait(
                lock, [&]()
                {
                    pred   = predic();
                    flag_c = flagCheck && getFlag() >= target;
                    return pred || flag_c;
                });
        if (m != nullptr)
        {
            m(*this, false, pred, flagCheck, target);
        }
    } {
        std::unique_lock<std::mutex> threadLock(_mutexs[_wait_count]);
        _waiting_mutexs.push(i);
    }
    _conditions[_wait_count].notify_one();
}

void SyncPoint::init()
{
    for (size_t i = 0; i < _wait_count; ++i)
    {
        _waiting_mutexs.push(i);
    }
}

int SyncPoint::_lock_mutex()
{
    _conditions[_wait_count].notify_one();
    std::unique_lock<std::mutex> lock(_mutexs[_wait_count]);
    _conditions[_wait_count].wait(
            lock, [&]()
            {
                return !_waiting_mutexs.empty();
            });
    int r = _waiting_mutexs.front();
    _waiting_mutexs.pop();
    return r;
}
}