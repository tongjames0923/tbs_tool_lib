//
// Created by abstergo on 24-11-13.
//
#include "concurrency/lock_operators/MutexLockOpeartor.h"
#include <time_utils.hpp>
#include <condition_variable>

void MutexLockOperator::lock(std::mutex &m) {
  m.lock();
  std::lock_guard<std::recursive_mutex> g(m_mutex);
  m_locks.insert_or_assign(&m, LockInfo<std::mutex>{std::this_thread::get_id(), true, &m});
}

bool MutexLockOperator::try_lock(std::mutex &m, unsigned long long int timeout) {
  std::condition_variable cv;
  std::mutex mtx;
  std::unique_lock<std::mutex> ul(mtx);
  auto ret = cv.wait_for(ul, time_utils::ms(timeout), [&]() { return m.try_lock(); });
  if (ret) {
	std::lock_guard<std::recursive_mutex> g(m_mutex);
	m_locks.insert_or_assign(&m, LockInfo<std::mutex>{std::this_thread::get_id(), true, &m});
  }

  return ret;
}

void MutexLockOperator::unlock(std::mutex &m) {
  m.unlock();
  std::lock_guard<std::recursive_mutex> g(m_mutex);
  m_locks.insert_or_assign(&m, LockInfo<std::mutex>{std::this_thread::get_id(), false, &m});
}

bool MutexLockOperator::locked(std::mutex &m) CONST {
  std::lock_guard<std::recursive_mutex> g(m_mutex);
  if (m_locks.contains(&m)) return m_locks.at(&m).locked;
  return false;
}

bool MutexLockOperator::heldByCurrentThread(std::mutex &m) CONST {
  std::lock_guard<std::recursive_mutex> g(m_mutex);
  if (m_locks.contains(&m)) {
	auto item = m_locks.at(&m);
	return item.locked && item.owner == std::this_thread::get_id();
  }
  return false;
}