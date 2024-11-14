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
#include <time_utils.hpp>

template<typename T>
struct LockInfo {
  std::thread::id owner;
  bool locked = false;
  T *object;
};

template<typename T>
class MappedLockOperator : public virtual tbs::concurrency::Lockable<T> {
private:
  std::unordered_map<T *, LockInfo<T>> m_locks;
  mutable std::recursive_mutex m_mutex;
public:
  MappedLockOperator() = default;

  virtual ~MappedLockOperator() = default;

  bool heldByCurrentThread(T &m) CONST override {
	std::lock_guard<std::recursive_mutex> g(m_mutex);
	if (m_locks.contains(&m)) {
	  auto item = m_locks.at(&m);
	  return item.locked && item.owner == std::this_thread::get_id();
	}
	return false;
  }

  bool locked(T &m) CONST override {
	std::lock_guard<std::recursive_mutex> g(m_mutex);
	if (m_locks.contains(&m)) return m_locks.at(&m).locked;
	return false;
  }

  bool try_lock(T &m, unsigned long long timeout) override {
	std::condition_variable cv;
	std::mutex mtx;
	std::unique_lock<std::mutex> ul(mtx);
	auto ret = cv.wait_for(ul, time_utils::ms(timeout), [&]() { return m.try_lock(); });
	if (ret) {
	  std::lock_guard<std::recursive_mutex> g(m_mutex);
	  m_locks.insert_or_assign(&m, LockInfo<T>{std::this_thread::get_id(), true, &m});
	}

	return ret;
  }

  void lock(T &m) override {
	m.lock();
	std::lock_guard<std::recursive_mutex> g(m_mutex);
	m_locks.insert_or_assign(&m, LockInfo<T>{std::this_thread::get_id(), true, &m});
  }

  void unlock(T &m) override {
	m.unlock();
	std::lock_guard<std::recursive_mutex> g(m_mutex);
	m_locks.insert_or_assign(&m, LockInfo<T>{std::this_thread::get_id(), false, &m});
  }

};


#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_MUTEXLOCKOPEARTOR_H
