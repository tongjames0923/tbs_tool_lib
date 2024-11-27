//
// Created by abstergo on 24-11-17.
//

#ifndef MAPPEDSHAREDLOCKOPERATOR_H
#define MAPPEDSHAREDLOCKOPERATOR_H
#include <tbs/concurrency/ParameterableLockAdapter.h>
#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <tbs/time_utils.hpp>

/**
 * @brief MappedSharedLockOperator 是一个模板类，用于管理共享锁和独占锁的操作。
 *
 * 该类继承自 AbstractTwoWaysLockAble，并提供了对共享锁和独占锁的详细管理功能。
 * 它使用 std::shared_mutex 类型的锁，并维护两个哈希表来记录共享锁和独占锁的状态。
 *
 * @tparam T 锁的类型，必须是 std::shared_mutex 或其派生类型。
 */
template<typename T>
class MappedSharedLockOperator : public virtual tbs::concurrency::AbstractTwoWaysLockAble<T>
{
    private:
        // 静态断言，确保 T 类型必须是 std::shared_mutex 的派生类型。
        static_assert(std::is_base_of_v<std::shared_mutex, T>, "T must be a std::shared_mutex");

        // 记录共享锁的信息，键为锁的指针，值为持有该锁的线程集合。
        std::unordered_map<T *, std::unordered_set<std::thread::id> > _shared_locks{};

        // 记录独占锁的信息，键为锁的指针，值为持有该锁的线程ID。
        std::unordered_map<T *, std::thread::id> _unique_locks{};

        // 互斥锁，用于保护共享锁信息的访问。
        mutable std::recursive_mutex _mutex_shared_locked;

        // 互斥锁，用于保护独占锁信息的访问。
        mutable std::recursive_mutex _mutex_unique_locked;

        /**
         * @brief 添加共享锁的信息。
         *
         * 该函数会在共享锁信息表中添加当前线程的ID。
         * 如果锁已经存在且当前线程已经持有该锁，则会抛出异常。
         *
         * @param l 锁对象。
         */
        void add_shared_lock_info(T &l)
        {
            std::lock_guard<std::recursive_mutex> g(_mutex_shared_locked);
            if (!_shared_locks.contains(&l))
            {
                std::unordered_set<std::thread::id> s;
                s.insert(std::this_thread::get_id());
                _shared_locks.insert({&l, s});
            }
            else
            {
                if (auto &s = _shared_locks.at(&l); s.contains(std::this_thread::get_id()))
                {
                    s.insert(std::this_thread::get_id());
                }
                else
                {
                    throw std::runtime_error("lock already exists");
                }
            }
        }

        /**
         * @brief 添加独占锁的信息。
         *
         * 该函数会在独占锁信息表中添加当前线程的ID。
         * 如果锁已经存在，则会抛出异常。
         *
         * @param l 锁对象。
         */
        void add_unique_lock_info(T &l)
        {
            std::lock_guard<std::recursive_mutex> g(_mutex_unique_locked);
            if (!_unique_locks.contains(&l))
            {
                _unique_locks.insert({&l, std::this_thread::get_id()});
            }
            else
            {
                throw std::runtime_error("lock already exists");
            }
        }

        /**
         * @brief 移除共享锁的信息。
         *
         * 该函数会在共享锁信息表中移除当前线程的ID。
         * 如果锁不存在或当前线程未持有该锁，则会抛出异常。
         *
         * @param l 锁对象。
         */
        void remove_shared_lock_info(T &l)
        {
            std::lock_guard<std::recursive_mutex> g(_mutex_shared_locked);
            if (_shared_locks.contains(&l))
            {
                auto &s  = _shared_locks.at(&l);
                auto  ec = s.erase(std::this_thread::get_id());
                if (ec == 0)
                {
                    throw std::runtime_error("shared lock on this thread not found");
                }
                if (s.empty())
                {
                    _shared_locks.erase(&l);
                }
            }
            else
            {
                throw std::runtime_error("shared lock not found");
            }

        }

        /**
         * @brief 移除独占锁的信息。
         *
         * 该函数会在独占锁信息表中移除当前线程的ID。
         * 如果锁不存在，则会抛出异常。
         *
         * @param l 锁对象。
         */
        void remove_unique_lock_info(T &l)
        {
            std::lock_guard<std::recursive_mutex> g(_mutex_unique_locked);
            if (_unique_locks.contains(&l))
            {
                _unique_locks.erase(&l);
            }
            else
            {
                throw std::runtime_error("unique lock not found");
            }
        }
    public:
        /**
         * @brief 根据参数锁定锁。
         *
         * 该函数根据参数 p 的值决定是锁定共享锁还是独占锁。
         *
         * @param l 锁对象。
         * @param p 参数，true 表示共享锁，false 表示独占锁。
         */
        void lockWithParameter(T &l, const bool &p) override
        {
            if (p)
            {
                l.lock_shared();
                add_shared_lock_info(l);
            }
            else
            {
                l.lock();
                add_unique_lock_info(l);
            }
        }

        /**
         * @brief 尝试根据参数锁定锁，并指定超时时间。
         *
         * 该函数根据参数 p 的值决定是尝试锁定共享锁还是独占锁，并指定超时时间。
         *
         * @param l 锁对象。
         * @param p 参数，true 表示共享锁，false 表示独占锁。
         * @param timeout 超时时间，以毫秒为单位。
         * @return true 如果成功锁定，否则返回 false。
         */
        bool tryLockWithParameter(T &l, const bool &p, unsigned long long timeout) override
        {
            bool ret = false;
            if constexpr (std::is_base_of_v<std::shared_timed_mutex, T>)
            {
                if (p)
                {
                    ret = l.try_lock_shared_for(time_utils::ms(timeout));
                }
                else
                {
                    ret = l.try_lock_for(time_utils::ms(timeout));
                }
                if (ret)
                {
                    add_shared_lock_info(l);
                }
            }
            else
            {
                // 否则，使用 condition_variable 和 mutex 来实现等待锁
                std::condition_variable      cv;
                std::mutex                   mtx;
                std::unique_lock<std::mutex> ul(mtx);
                ret = cv.wait_for(
                        ul, time_utils::ms(timeout), [&]()
                        {
                            if (p)
                            {
                                return l.try_lock_shared();
                            }
                            else
                            {
                                return l.try_lock();
                            }
                        });
                if (ret)
                {
                    add_unique_lock_info(l);
                }
            }
            return ret;
        }

        /**
         * @brief 根据参数解锁锁。
         *
         * 该函数根据参数 p 的值决定是解锁共享锁还是独占锁。
         *
         * @param l 锁对象。
         * @param p 参数，true 表示共享锁，false 表示独占锁。
         */
        void unlockWithParameter(T &l, const bool &p) override
        {
            if (p)
            {
                remove_shared_lock_info(l);
                l.unlock_shared();
            }
            else
            {
                remove_unique_lock_info(l);
                l.unlock();

            }
        }

        /**
         * @brief 检查锁是否被锁定。
         *
         * 该函数根据参数 p 的值决定是检查共享锁还是独占锁的状态。
         *
         * @param l 锁对象。
         * @param p 参数，true 表示共享锁，false 表示独占锁。
         * @return true 如果锁被锁定，否则返回 false。
         */
        bool lockedWithParameter(T &l, const bool &p) CONST override
        {
            if (p)
            {
                std::lock_guard<std::recursive_mutex> g(_mutex_shared_locked);
                return _shared_locks.contains(&l);
            }
            else
            {
                std::lock_guard<std::recursive_mutex> g(_mutex_unique_locked);
                return _unique_locks.contains(&l);
            }
        }

        /**
         * @brief 检查当前线程是否持有锁。
         *
         * 该函数根据参数 p 的值决定是检查当前线程是否持有共享锁还是独占锁。
         *
         * @param l 锁对象。
         * @param p 参数，true 表示共享锁，false 表示独占锁。
         * @return true 如果当前线程持有锁，否则返回 false。
         */
        bool heldByCurrentThreadWithParameter(T &l, const bool &p) CONST override
        {
            if (p)
            {
                std::lock_guard<std::recursive_mutex> g(_mutex_shared_locked);
                if (_shared_locks.contains(&l))
                {
                    return _shared_locks.at(&l).contains(std::this_thread::get_id());
                }
                return false;
            }
            else
            {
                std::lock_guard<std::recursive_mutex> g(_mutex_unique_locked);
                if (_unique_locks.contains(&l))
                {
                    return _unique_locks.at(&l) == std::this_thread::get_id();
                }
                return false;
            }
        }

        /**
         * @brief 获取独占锁参数。
         *
         * 该函数返回 false，表示默认情况下不使用独占锁参数。
         *
         * @return false。
         */
        constexpr bool getUniqueLockParameter() CONST override
        {
            return false;
        }
};

#endif //MAPPEDSHAREDLOCKOPERATOR_H