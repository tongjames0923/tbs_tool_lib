#include <tbs/concurrency/sync_point/SyncPoint.h>

#include <cstddef>
#include "SyncPointImpl_impls.cpp"

namespace tbs::concurrency::sync_point
{

    size_t SyncPoint::waitingCount() const
    {
        return getImpl()._wait_count - getImpl()._waiting_mutexs.size();
    }

    SyncPoint::SyncPoint(CONST size_t& wait_count) : PointerImpl(new SyncPointImpl(wait_count))
    {
        getImpl().init();
    }

    void SyncPoint::wait_to_predicate(__predic_function f, __on_predic_moment m)
    {
        wait_to_predicate([&]() { return f(); }, std::move(m));
    }

    void SyncPoint::wait_to_predicate(__predic_functional f, __on_predic_moment m)
    {
        getImpl().wait(time_utils::ms(0), *this, std::move(f), false, -1, false, std::move(m));
    }

    void SyncPoint::wait_until(const time_utils::ms& to, __on_predic_moment m)
    {
        wait_until(to, [&]() { return false; }, std::move(m));
    }

    void SyncPoint::wait_until(const time_utils::ms& timeout, __predic_functional f, __on_predic_moment m)
    {
        getImpl().wait(timeout, *this, std::move(f), false, -1, true, std::move(m));
    }

    void SyncPoint::reset()
    {
        getImpl()._flag = 0;
        wakeup();
    }

    void SyncPoint::wait_flag(const int& target, __on_predic_moment m)
    {
        wait_flag(target, [&]() { return false; }, std::move(m));
    }

    void SyncPoint::wait_flag(const int& target, __predic_function f, __on_predic_moment m)
    {
        wait_flag(target, [&]() { return f(); }, std::move(m));
    }

    void SyncPoint::wait_flag(const int& target, __predic_functional f, __on_predic_moment m)
    {
        getImpl().wait(time_utils::ms(0), *this, std::move(f), true, target, false, std::move(m));
    }

    void SyncPoint::wait_flag(const int& target, const time_utils::ms& m, __on_predic_moment m_on_predic_moment)
    {
        wait_flag(target, m, [&]() { return false; }, std::move(m_on_predic_moment));
    }

    void SyncPoint::wait_flag(const int& target, const time_utils::ms& ms, __predic_function f, __on_predic_moment m)
    {
        wait_flag(target, ms, [&]() { return f(); }, std::move(m));
    }

    void SyncPoint::wait_flag(const int& target, const time_utils::ms& ms, __predic_functional f, __on_predic_moment m)
    {
        getImpl().wait(ms, *this, std::move(f), true, target, true, std::move(m));
    }

    int SyncPoint::accumulateFlag(const int& delta)
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
} // namespace tbs::concurrency::sync_point
