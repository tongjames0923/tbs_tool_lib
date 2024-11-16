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
        unsigned long long ms, __predic_functional predic, CONST bool &flagCheck, CONST int &target,
        CONST bool &       timeLimited)
{
    int                          i = _lock_mutex();
    std::unique_lock<std::mutex> lock(_mutexs[i]);
    if (timeLimited)
    {
        _conditions[i].wait_for(
                lock, time_utils::ms(ms), [&]()
                {
                    return predic() || (flagCheck && getFlag() >= target);
                });
    }
    else
    {
        _conditions[i].wait(
                lock, [&]()
                {
                    return predic() || (flagCheck && getFlag() >= target);
                });
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