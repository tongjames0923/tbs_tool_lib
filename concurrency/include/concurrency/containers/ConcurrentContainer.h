//
// Created by abstergo on 24-11-18.
//

#ifndef CONCURRENTCONTAINER_H
#define CONCURRENTCONTAINER_H

#include <concurrency/adapters.h>
#include <concurrency/sync_point/SyncPoint.h>


namespace tbs::concurrency::container {

template<typename CONTAINER, typename LOCK_ADAPTER>
class ConcurrentContainer {
private:
  CONTAINER m_container;
  mutable LOCK_ADAPTER m_lock;
  using _lock_guard = guard::auto_op_lock_guard<LOCK_ADAPTER>; // 定义锁的智能指针类型

  /**
   * @brief 静态常量，用于判断是否使用了共享锁
   */
  constexpr static bool __is_shared_lock = std::is_base_of_v<SharedMutexLockAdapter, LOCK_ADAPTER> ||
                                           std::is_base_of_v<SharedTimedMutexLockAdapter, LOCK_ADAPTER>;


  using _read_Func = std::function<void(CONST CONTAINER &)>;
  using _write_Func = std::function<void(CONTAINER &)>;
protected:
  void readAsAtomic(_read_Func f) CONST {
    if constexpr (__is_shared_lock) {
      using _shared_lock_guard = guard::auto_shared_lock_op_guard<LOCK_ADAPTER>;
      _shared_lock_guard g(m_lock);
    } else {
      _lock_guard g(m_lock);
    }
    f(m_container);
  }

  void writeAsAtomic(_write_Func f) {
    _lock_guard g(m_lock);
    f(m_container);
  }

public:
  virtual ~ConcurrentContainer() = default;

  ConcurrentContainer(ConcurrentContainer &&o) noexcept {
    m_container = std::move(o.m_container);
  }

  ConcurrentContainer &operator=(ConcurrentContainer &&o) noexcept {
    m_container = std::move(o.m_container);
  }

  ConcurrentContainer(CONST ConcurrentContainer &o) {
    m_container = o.m_container;
  }

  ConcurrentContainer &operator=(CONST ConcurrentContainer &o) {
    m_container = o.m_container;
  }

  ConcurrentContainer() = default;

};
}

#endif //CONCURRENTCONTAINER_H