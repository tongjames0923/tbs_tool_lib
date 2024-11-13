//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H

#include <concurrency/lock_operators/MutexLockOpeartor.h>

using mutex_lock_adapter = tbs::concurrency::LockAdapter<std::mutex, MutexLockOperator>;


#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCK_OPERATORS_OPERATORS_H
