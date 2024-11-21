//
// Created by abstergo on 24-11-15.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H

#include <atomic>
#include <condition_variable>
#include <defs.h>
#include <functional>
#include <queue>
#include <time_utils.hpp>
#include <utility>

#include "PointerToImpl.h"
/**
 * 命名空间 tbs::concurrency::sync_point 用于同步点机制，提供线程同步的工具类。
 */
namespace tbs::concurrency::sync_point
{
/**
 * 类型别名 __predic_function 表示旧式的谓词函数指针类型。
 */
using __predic_function = bool (*)();

/**
 * 类型别名 __predic_functional 表示现代 C++ 风格的谓词函数类型。
 */
using __predic_functional = std::function<bool()>;

class SyncPoint;

/**
 * 类型别名 __on_predic_moment 表示等待条件达成时的回调函数。
 * @param point 当前的 SyncPoint 对象。
 * @param isTimeouted 是否超时。
 * @param isPredicted 是否是预期条件达成。
 * @param isFlagCheck 是否检查内部标志。
 * @param targetFlag 目标标志值。
 */
using __on_predic_moment = std::function<void(
        CONST SyncPoint &point, bool isTimeouted, bool isPredicted, bool isFlagCheck, CONST int &targetFlag)>;


class SyncPointImpl;


/**
 * SyncPoint 类用于线程同步，提供多种等待条件和标志的机制。
 *
 */
class SyncPoint : public virtual PointerImpl<SyncPointImpl>

{
    public:
        /**
         * 可用等待的互斥锁数量。
         * @return 内部标志。
         */
        size_t waitingCount() const;

        /**
         * 构造函数，初始化等待队列。
         * @param waitCount 可用等待的互斥锁数量，默认为 4。
         */
        explicit SyncPoint(CONST size_t &waitCount = 4);

        /**
         * 等待直到指定的旧式谓词函数返回 true。
         * @param f 旧式谓词函数。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_to_predicate(__predic_function f, __on_predic_moment m = nullptr);

        /**
         * 等待直到指定的现代谓词函数返回 true。
         * @param f 现代谓词函数。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_to_predicate(__predic_functional f, __on_predic_moment m = nullptr);
        /**
         * 等待直到指定的时间。
         * @param to 超时时间。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_until(CONST time_utils::ms &to, __on_predic_moment m = nullptr);

        /**
         * 等待直到指定的现代谓词函数返回 true 或超时。
         * @param timeout 超时时间。
         * @param f 现代谓词函数。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_until(
                CONST time_utils::ms &timeout, __predic_functional f = []()
                {
                    return false;
                },
                __on_predic_moment m = nullptr);

        /**
         * 重置内部标志为 0。
         */
        void reset();

        /**
         * 等待直到内部标志达到目标值。
         * @param target 目标标志值。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_flag(CONST int &target, __on_predic_moment m = nullptr);

        /**
         * 等待直到内部标志达到目标值或谓词函数返回 true。
         * @param target 目标标志值。
         * @param f 旧式谓词函数。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_flag(CONST int &target, __predic_function f, __on_predic_moment m = nullptr);

        /**
         * 等待直到内部标志达到目标值或现代谓词函数返回 true。
         * @param target 目标标志值。
         * @param f 现代谓词函数。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_flag(CONST int &target, __predic_functional f, __on_predic_moment m = nullptr);

        /**
         * 等待直到内部标志达到目标值或超时。
         * @param target 目标标志值。
         * @param m 超时时间。
         * @param m_on_predic_moment 回调函数，在等待条件达成时调用。
         */
        void wait_flag(CONST int &target, CONST time_utils::ms &m, __on_predic_moment m_on_predic_moment = nullptr);

        /**
         * 等待直到内部标志达到目标值、超时或谓词函数返回 true。
         * @param target 目标标志值。
         * @param ms 超时时间。
         * @param f 旧式谓词函数。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_flag(
                CONST int &target, CONST time_utils::ms &ms, __predic_function f, __on_predic_moment m = nullptr);

        /**
         * 等待直到内部标志达到目标值、超时或现代谓词函数返回 true。
         * @param target 目标标志值。
         * @param ms 超时时间。
         * @param f 现代谓词函数。
         * @param m 回调函数，在等待条件达成时调用。
         */
        void wait_flag(
                CONST int &target, CONST time_utils::ms &ms, __predic_functional f, __on_predic_moment m = nullptr);

        /**
         * 增加内部标志的值并通知一个等待的线程。
         * @param delta 增加的值。
         * @return 增加后的标志值。
         */
        int accumulateFlag(const int &delta);

        /**
         * 获取当前的内部标志值。
         * @return 当前的标志值。
         */
        [[nodiscard]] int getFlag() const;
        /**
         * 唤醒所有等待的线程。防止等待睡死
         */
        void wakeup();

        void operator()(SyncPointImpl *ptr) override;

        DELETE_COPY_ASSIGNMENT(SyncPoint)
        DELETE_COPY_CONSTRUCTION(SyncPoint)
        DEFAULT_MOVE_ASSIGNMENT(SyncPoint)
        DEFAULT_MOVE_CONSTRUCTION(SyncPoint)
};

};

#endif // TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_SYNC_POINT_SYNCPOINT_H