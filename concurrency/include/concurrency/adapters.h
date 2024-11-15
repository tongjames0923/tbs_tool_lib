//
// Created by abstergo on 24-11-14.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H

#include <concurrency/lock_operators/operators.h>
#include <concurrency/LockAdapter.h>

using MutexLockAdapter     = tbs::concurrency::UniqueLockAdapter<std::mutex, MutexLockOpeartor>;
using RecursiveLockAdapter = tbs::concurrency::UniqueLockAdapter<std::recursive_mutex, RecursiveLockOperator>;

using TimedMutexLockAdapter          = tbs::concurrency::UniqueLockAdapter<std::timed_mutex, TimedMutexLockOperator>;
using RecursiveTimedMutexLockAdapter = tbs::concurrency::UniqueLockAdapter<std::recursive_timed_mutex,
                                                                           RecursiveTimedLockOperator>;

#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H