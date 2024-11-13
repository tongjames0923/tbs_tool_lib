//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H

#include <concurrency/LockAdapter.h>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <atomic>

template<typename T>
struct LockInfo {
  std::thread::id owner;
  bool locked = false;
  T *object;
};

class MutexLockOperator : public virtual tbs::concurrency::Lockable<std::mutex> {
private:
  std::unordered_map<std::mutex *, LockInfo<std::mutex>> m_locks;
  mutable std::recursive_mutex m_mutex;
public:
  MutexLockOperator() = default;

  ~MutexLockOperator() = default;

  void lock(std::mutex &) override;

  bool try_lock(std::mutex &, unsigned long long) override;

  void unlock(std::mutex &) override;

  bool locked(std::mutex &) CONST override;

  bool heldByCurrentThread(std::mutex &) CONST override;
};

#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H
