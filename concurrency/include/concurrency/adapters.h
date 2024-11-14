//
// Created by abstergo on 24-11-14.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H

#include <concurrency/lock_operators/operators.h>
#include <concurrency/LockAdapter.h>


using MutexLockAdapter = tbs::concurrency::LockAdapter<std::mutex, MutexLockOpeartor>;
using RecursiveLockAdapter = tbs::concurrency::LockAdapter<std::recursive_mutex, RecursiveLockOperator>;


#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_ADAPTERS_H
