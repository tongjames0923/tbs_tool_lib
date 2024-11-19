//
// Created by abstergo on 24-11-14.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H

#include <concurrency/LockAdapter.h>
#include <concurrency/ParameterableLockAdapter.h>
#include <concurrency/lock_operators/operators.h>
#include <shared_mutex>

/**
 * @brief 定义互斥锁的适配器类型，提供唯一的锁操作接口。
 */
using MutexLockAdapter = tbs::concurrency::UniqueLockAdapter<std::mutex, MutexLockOpeartor>;

/**
 * @brief 定义递归互斥锁的适配器类型，支持递归加锁操作。
 */
using RecursiveLockAdapter = tbs::concurrency::UniqueLockAdapter<std::recursive_mutex, RecursiveLockOperator>;

/**
 * @brief 定义带超时功能的互斥锁适配器类型，支持在指定时间内尝试加锁。
 */
using TimedMutexLockAdapter = tbs::concurrency::UniqueLockAdapter<std::timed_mutex, TimedMutexLockOperator>;

/**
 * @brief 定义带超时功能的递归互斥锁适配器类型，支持递归加锁并在指定时间内尝试加锁。
 */
using RecursiveTimedMutexLockAdapter =
        tbs::concurrency::UniqueLockAdapter<std::recursive_timed_mutex, RecursiveTimedLockOperator>;

/**
 * @brief 定义共享锁的适配器类型，支持多个读取者同时访问资源。
 */
using SharedMutexLockAdapter =
        tbs::concurrency::SharedLockAdapter<std::shared_mutex, MappedSharedLockOperator<std::shared_mutex> >;

/**
 * @brief 定义带超时功能的共享锁适配器类型，支持多个读取者同时访问资源并在指定时间内尝试加锁。
 */
using SharedTimedMutexLockAdapter = tbs::concurrency::SharedLockAdapter<
    std::shared
#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H