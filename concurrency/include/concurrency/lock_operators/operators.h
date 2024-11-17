//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H

#include <concurrency/lock_operators/MappedLockOperator.h>
#include <concurrency/lock_operators/MappedSharedLockOperator.h>

// 定义用于操作普通互斥锁的别名
using MutexLockOpeartor = MappedLockOperator<std::mutex>;

// 定义用于操作递归互斥锁的别名
using RecursiveLockOperator = MappedLockOperator<std::recursive_mutex>;

// 定义用于操作带时间限制的互斥锁的别名
using TimedMutexLockOperator = MappedLockOperator<std::timed_mutex>;

// 定义用于操作带时间限制的递归互斥锁的别名
using RecursiveTimedLockOperator = MappedLockOperator<std::recursive_timed_mutex>;

// 定义用于操作共享互斥锁的别名
using SharedLockOperator = MappedSharedLockOperator<std::shared_mutex>;

// 定义用于操作带时间限制的共享互斥锁的别名
using SharedTimedLockOperator = MappedSharedLockOperator<std::shared_timed_mutex>;

#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H