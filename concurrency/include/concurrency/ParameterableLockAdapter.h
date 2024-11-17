//
// Created by abstergo on 24-11-17.
//

#ifndef SHAREDLOCKADAPTER_H
#define SHAREDLOCKADAPTER_H
#include <concurrency/LockAdapter.h>



namespace tbs::concurrency
{

/**
 * @brief 参数化可锁对象的虚基类，继承自 tbs::concurrency::Lockable<T>
 *
 * @tparam T 锁类型
 * @tparam P 参数类型
 */
template<typename T, typename P>
class ParameterableLockable : public virtual tbs::concurrency::Lockable<T>
{
    public:
        /**
         * @brief 使用特定参数锁定对象
         *
         * @param l 锁对象
         * @param p 锁参数
         */
        virtual void lockWithParameter(T &l, CONST P &p) = 0;

        /**
         * @brief 尝试使用特定参数锁定对象，带有超时设置
         *
         * @param l 锁对象
         * @param p 锁参数
         * @param timeout 超时时间（毫秒）
         * @return 成功锁定返回 true，否则返回 false
         */
        virtual bool tryLockWithParameter(T &l, CONST P &p, unsigned long long timeout) = 0;

        /**
         * @brief 使用特定参数解锁对象
         *
         * @param l 锁对象
         * @param p 锁参数
         */
        virtual void unlockWithParameter(T &l, CONST P &p) = 0;

        /**
         * @brief 检查对象是否使用特定参数锁定
         *
         * @param l 锁对象
         * @param p 锁参数
         * @return 锁定返回 true，否则返回 false
         */
        virtual bool lockedWithParameter(T &l, CONST P &p)CONST = 0;

        /**
         * @brief 检查当前线程是否使用特定参数持有锁
         *
         * @param l 锁对象
         * @param p 锁参数
         * @return 当前线程持有锁返回 true，否则返回 false
         */
        virtual bool heldByCurrentThreadWithParameter(T &l, CONST P &p)CONST = 0;

        /**
         * @brief 获取唯一锁参数
         *
         * @return 唯一锁参数
         */
        virtual constexpr P getUniqueLockParameter() CONST = 0;

        /**
         * @brief 实现 lock 函数，使用 getUniqueLockParameter() 获取参数后调用 lockWithParameter
         *
         * @param l 锁对象
         */
        void lock(T &l) override
        {
            lockWithParameter(l, getUniqueLockParameter());
        }

        /**
         * @brief 实现 unlock 函数，使用 getUniqueLockParameter() 获取参数后调用 unlockWithParameter
         *
         * @param l 锁对象
         */
        void unlock(T &l) override
        {
            unlockWithParameter(l, getUniqueLockParameter());
        }

        /**
         * @brief 实现 try_lock 函数，使用 getUniqueLockParameter() 获取参数后调用 tryLockWithParameter
         *
         * @param l 锁对象
         * @param timeout 超时时间（毫秒）
         * @return 成功锁定返回 true，否则返回 false
         */
        bool try_lock(T &l, unsigned long long timeout) override
        {
            return tryLockWithParameter(l, getUniqueLockParameter(), timeout);
        }

        /**
         * @brief 实现 locked 函数，使用 getUniqueLockParameter() 获取参数后调用 lockedWithParameter
         *
         * @param l 锁对象
         * @return 锁定返回 true，否则返回 false
         */
        bool locked(T &l) const override
        {
            return lockedWithParameter(l, getUniqueLockParameter());
        }

        /**
         * @brief 实现 heldByCurrentThread 函数，使用 getUniqueLockParameter() 获取参数后调用 heldByCurrentThreadWithParameter
         *
         * @param l 锁对象
         * @return 当前线程持有锁返回 true，否则返回 false
         */
        bool heldByCurrentThread(T &l) const override
        {
            return heldByCurrentThreadWithParameter(l, getUniqueLockParameter());
        }
};


// 定义一个模板别名AbstractTwoWaysLockAble，用于表示一个具有双向锁定能力的抽象类型
// 其中T为锁定的类型，bool表示是否锁定
template<typename T>
using AbstractTwoWaysLockAble = ParameterableLockable<T, bool>;

/**
 * @brief SharedLockAdapter是一个模板类，用于适配共享锁的使用。
 *
 * 该类继承自tbs::concurrency::UniqueLockAdapter<L, OP>，并提供了与共享锁相关的操作。
 * 它通过使用操作参数来区分共享锁和独占锁，从而在相同的锁对象上实现不同的锁行为。
 *
 * @tparam L 锁的类型，需要与UniqueLockAdapter中使用的锁类型一致。
 * @tparam OP 锁操作的类型，必须继承自AbstractTwoWaysLockAble。
 */
template<typename L, typename OP>
class SharedLockAdapter : public virtual tbs::concurrency::UniqueLockAdapter<L, OP>
{
    // 确保OP类型继承自AbstractTwoWaysLockAble，以保证其支持共享锁和独占锁的操作。
    static_assert(
            std::is_base_of_v<AbstractTwoWaysLockAble<L>, OP>,
            "OP must base on AbstractTwoWaysLockAble for sharedLockAdapter");
    public:
        // 定义共享锁和独占锁的标志常量。
        constexpr static bool SHARED_LOCK_FLAG = true, UNIQUE_LOCK_FLAG = false;

        /**
         * @brief 获取共享锁。
         *
         * 该函数调用底层操作对象的lockWithParameter方法，并传递共享锁标志。
         */
        void lockShared()
        {
            this->m_op.lockWithParameter(this->m_lock, SHARED_LOCK_FLAG);
        }

        /**
         * @brief 释放共享锁。
         *
         * 该函数调用底层操作对象的unlockWithParameter方法，并传递共享锁标志。
         */
        void unlockShared()
        {
            this->m_op.unlockWithParameter(this->m_lock, SHARED_LOCK_FLAG);
        }

        /**
         * @brief 尝试获取共享锁，并指定超时时间。
         *
         * 该函数调用底层操作对象的tryLockWithParameter方法，并传递共享锁标志和超时时间。
         *
         * @param timeout 超时时间，以毫秒为单位。
         * @return true 如果成功获取共享锁，否则返回false。
         */
        bool tryLockShared(unsigned long long timeout)
        {
            return this->m_op.tryLockWithParameter(this->m_lock, SHARED_LOCK_FLAG, timeout);
        }

        /**
         * @brief 检查锁是否为共享锁状态。
         *
         * 该函数调用底层操作对象的lockedWithParameter方法，并传递共享锁标志。
         *
         * @return true 如果锁处于共享锁状态，否则返回false。
         */
        bool lockedShared()CONST
        {
            return this->m_op.lockedWithParameter(this->m_lock, SHARED_LOCK_FLAG);
        }

        /**
         * @brief 检查当前线程是否持有共享锁。
         *
         * 该函数调用底层操作对象的heldByCurrentThreadWithParameter方法，并传递共享锁标志。
         *
         * @return true 如果当前线程持有共享锁，否则返回false。
         */
        bool heldByCurrentThreadShared()CONST
        {
            return this->m_op.heldByCurrentThreadWithParameter(this->m_lock, SHARED_LOCK_FLAG);
        }

};



namespace guard
{
    namespace functions
    {
        template<typename T>
        static inline void shared_lock_on_init(T &t)
        {
            t.lockShared();
        }

        template<typename T>
        static inline void shared_on_destroy(T &t)
        {
            t.unlockShared();
        }
    }



    template<typename T>
    using auto_shared_lock_op_guard = Guard<T, functions::shared_lock_on_init<T>, functions::shared_on_destroy<T> >;

}
}

#endif //SHAREDLOCKADAPTER_H