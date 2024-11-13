//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCKADAPTER_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCKADAPTER_H

#include <defs.h>
#include <unordered_map>

namespace tbs::concurrency {
template<typename T>
class Lockable {
public:
  virtual void lock(T &) = 0;

  virtual void unlock(T &) = 0;

  virtual bool try_lock(T &, unsigned long long) = 0;

  virtual bool locked(T &) CONST = 0;

  virtual bool heldByCurrentThread(T &) CONST = 0;
};


template<typename T, typename OP>
class LockAdapter {
private:
  T m_lock;
  OP m_op;
public:
  LockAdapter(T lock) : m_lock(std::move(lock)) {

  }

  LockAdapter(T lock, OP op) : m_lock(std::move(lock)), m_op(std::move(op)) {

  }

  LockAdapter(const LockAdapter &) = delete;

  LockAdapter &operator=(const LockAdapter &) = delete;

  LockAdapter(LockAdapter &&) = default;

  LockAdapter &operator=(LockAdapter &&) = default;

  LockAdapter() = default;

  virtual ~LockAdapter() {

  }

  T getLock() const {
	return m_lock;
  }

  void lock() {
	m_op.lock(m_lock);
  }

  void unlock() {
	m_op.unlock(m_lock);
  }

  bool try_lock(unsigned long long t) {
	return m_op.try_lock(m_lock, t);
  }

  bool locked() CONST {
	return m_op.locked(m_lock);
  }

  bool heldByCurrentThread() CONST {
	return m_op.heldByCurrentThread(m_lock);
  }
};


}


#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCKADAPTER_H
