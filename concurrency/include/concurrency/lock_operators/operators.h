//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H

#include <concurrency/lock_operators/MappedLockOperator.h>

using MutexLockOpeartor = MappedLockOperator<std::mutex>;
using RecursiveLockOperator = MappedLockOperator<std::recursive_mutex>;
using TimedMutexLockOperator = MappedLockOperator<std::timed_mutex>;
using RecursiveTimedLockOperator = MappedLockOperator<std::recursive_timed_mutex>;


#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H
