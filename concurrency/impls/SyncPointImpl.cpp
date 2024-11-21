#include <concurrency/sync_point/SyncPoint.h>
#include <cstddef>


namespace tbs::concurrency::sync_point
{
class SyncPointImpl
{
    public:
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
    public:
        void init()
        {
            for (size_t i = 0; i < _wait_count; ++i)
            {
                _waiting_mutexs.push(i);
            }
        }

        void wait(
                CONST time_utils::ms &ms, SyncPoint &sp, __predic_functional predic, CONST bool &flagCheck,
                CONST int &           target,
                CONST bool &          timeLimited, __on_predic_moment m)
        {
            const size_t i = _lock_mutex();
            if (timeLimited)
            {
                bool                         pred = false, flag_c = false;
                std::unique_lock<std::mutex> lock(_mutexs[i]);
                bool                         r = _conditions[i].wait_for(
                        lock, time_utils::ms(ms),
                        [&]()
                        {
                            pred   = predic();
                            flag_c = flagCheck && _flag >= target;
                            return pred || flag_c;
                        });
                if (m != nullptr)
                {
                    m(sp, !r, pred, flag_c, target);
                }
            }
            else
            {
                bool                         pred = false, flag_c = false;
                std::unique_lock<std::mutex> lock(_mutexs[i]);
                _conditions[i].wait(
                        lock,
                        [&]()
                        {
                            pred   = predic();
                            flag_c = flagCheck && _flag >= target;
                            return pred || flag_c;
                        });
                if (m != nullptr)
                {
                    m(sp, false, pred, flagCheck, target);
                }
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
            _conditions[_wait_count].wait(
                    lock, [&]()
                    {
                        return !_waiting_mutexs.empty();
                    });
            const size_t r = _waiting_mutexs.front();
            _waiting_mutexs.pop();
            return r;
        }

        void operator()(SyncPointImpl *ptr) const
        {
            delete ptr;
            ptr = nullptr;
        }

};


size_t SyncPoint::waitingCount() const
{
    return getImpl()._wait_count - getImpl()._waiting_mutexs.size();
}

SyncPoint::SyncPoint(CONST size_t &wait_count) : PointerImpl(new SyncPointImpl(wait_count))
{
    getImpl().init();
}

void SyncPoint::wait_to_predicate(__predic_function f, __on_predic_moment m)
{
    wait_to_predicate(
            [&]()
            {
                return f();
            }, std::move(m));
}

void SyncPoint::wait_to_predicate(__predic_functional f, __on_predic_moment m)
{
    getImpl().wait(time_utils::ms(0), *this, std::move(f), false, -1, false, std::move(m));
}

void SyncPoint::wait_until(const time_utils::ms &to, __on_predic_moment m)
{
    wait_until(
            to, [&]()
            {
                return false;
            }, std::move(m));
}

void SyncPoint::wait_until(const time_utils::ms &timeout, __predic_functional f, __on_predic_moment m)
{
    getImpl().wait(timeout, *this, std::move(f), false, -1, true, std::move(m));
}

void SyncPoint::reset()
{
    getImpl()._flag = 0;
    wakeup();
}

void SyncPoint::wait_flag(const int &target, __on_predic_moment m)
{
    wait_flag(
            target, [&]()
            {
                return false;
            }, std::move(m));
}

void SyncPoint::wait_flag(const int &target, __predic_function f, __on_predic_moment m)
{
    wait_flag(
            target, [&]()
            {
                return f();
            }, std::move(m));
}

void SyncPoint::wait_flag(const int &target, __predic_functional f, __on_predic_moment m)
{
    getImpl().wait(time_utils::ms(0), *this, std::move(f), true, target, false, std::move(m));
}

void SyncPoint::wait_flag(const int &target, const time_utils::ms &m, __on_predic_moment m_on_predic_moment)
{
    wait_flag(
            target, m, [&]()
            {
                return false;
            }, std::move(m_on_predic_moment));
}

void SyncPoint::wait_flag(const int &target, const time_utils::ms &ms, __predic_function f, __on_predic_moment m)
{
    wait_flag(
            target, ms, [&]()
            {
                return f();
            }, std::move(m));
}

void SyncPoint::wait_flag(const int &target, const time_utils::ms &ms, __predic_functional f, __on_predic_moment m)
{
    getImpl().wait(ms, *this, std::move(f), true, target, true, std::move(m));
}

int SyncPoint::accumulateFlag(const int &delta)
{
    const int k = getImpl()._flag += delta;
    wakeup();
    return k;
}

int SyncPoint::getFlag() const
{
    return getImpl()._flag;
}

void SyncPoint::wakeup()
{
    for (int i = 0; i < getImpl()._wait_count; i++)
    {
        getImpl()._conditions[i].notify_one();
    }
}

// void SyncPoint::operator()(SyncPointImpl *ptr)
// {
//     delete ptr;
//     ptr = nullptr;
// }


} // namespace tbs::concurrency::sync_point