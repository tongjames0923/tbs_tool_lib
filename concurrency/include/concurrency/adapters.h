//
// Created by abstergo on 24-11-14.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H

#include <shared_mutex>
#include <concurrency/lock_operators/operators.h>
#include <concurrency/LockAdapter.h>
#include <concurrency/ParameterableLockAdapter.h>

using MutexLockAdapter     = tbs::concurrency::UniqueLockAdapter<std::mutex, MutexLockOpeartor>;
using RecursiveLockAdapter = tbs::concurrency::UniqueLockAdapter<std::recursive_mutex, RecursiveLockOperator>;

using TimedMutexLockAdapter          = tbs::concurrency::UniqueLockAdapter<std::timed_mutex, TimedMutexLockOperator>;
using RecursiveTimedMutexLockAdapter = tbs::concurrency::UniqueLockAdapter<std::recursive_timed_mutex,
                                                                           RecursiveTimedLockOperator>;
using SharedMutexLockAdapter = tbs::concurrency::SharedLockAdapter<
    std::shared_mutex, MappedSharedLockOperator<std::shared_mutex> >;
using SharedTimedMutexLockAdapter = tbs::concurrency::SharedLockAdapter<
    std::shared_timed_mutex, MappedSharedLockOperator<std::shared_timed_mutex> >;

#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H