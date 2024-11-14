//
// Created by abstergo on 24-11-13.
//

#ifndef TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCKADAPTER_H
#define TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCKADAPTER_H
#include <guard.h>
#include <defs.h>
#include <unordered_map>

namespace tbs::concurrency {
/**
 * Lockable 模板类定义了对某种类型 T 的对象进行锁操作的接口。
 * 它提供了一组纯虚函数，用于实现锁的锁定、解锁和状态检查功能。
 * 这个类的目的是为不同类型的锁提供一个统一的接口，使得锁的使用更加灵活和通用。
 *
 * @tparam T 表示被锁定对象的类型。
 */
template<typename T>
class Lockable {
public:
  /**
   * 尝试锁定指定的资源。
   * 如果资源已经被其他线程锁定，那么调用此方法的线程将会阻塞，直到资源被释放。
   *
   * @param res 要锁定的资源。
   */
  virtual void lock(T &) = 0;

  /**
   * 解锁之前锁定的资源。
   * 调用此方法后，其他等待该资源的线程将会有机会获得锁并继续执行。
   *
   * @param res 要解锁的资源。
   */
  virtual void unlock(T &) = 0;

  /**
   * 尝试锁定资源，但如果资源已经被锁定，则立即返回而不阻塞。
   * 这个方法对于需要检查资源是否可用，但又不想等待的情况非常有用。
   *
   * @param res 要尝试锁定的资源。
   * @param timeout 尝试锁定资源的超时时间。单位毫秒
   * @return 如果成功锁定资源返回 true，否则返回 false。
   */
  virtual bool try_lock(T &, unsigned long long) = 0;

  /**
   * 检查指定的资源是否已经被锁定。
   * 这个方法用于在不尝试锁定资源的情况下，检查资源的锁定状态。
   *
   * @param res 要检查的资源。
   * @return 如果资源已被锁定返回 true，否则返回 false。
   */
  virtual bool locked(T &) CONST = 0;

  /**
   * 检查当前线程是否持有指定资源的锁。
   * 这个方法主要用于调试和断言，以确保在操作资源时线程是安全的。
   *
   * @param res 要检查的资源。
   * @return 如果当前线程持有资源的锁返回 true，否则返回 false。
   */
  virtual bool heldByCurrentThread(T &) CONST = 0;
};


/**
 * @brief UniqueLockAdapter类模板提供了一种通用的独占锁适配机制
 *
 * 该类模板封装了一个锁对象和一组锁操作，允许在不同的上下文中重用和适配不同的锁类型和操作
 * 它通过删除拷贝构造函数和赋值操作符来确保锁的唯一性，同时支持移动语义以提高性能
 *
 * @tparam T 锁对象的类型，通常是一个实现了锁接口的类
 * @tparam OP 锁操作的类型，包含对锁进行lock、unlock等操作的实现
 */
template<typename T, typename OP>
class UniqueLockAdapter {
private:
  T m_lock; ///< 封装的锁对象
  OP m_op; ///< 封装的锁操作对象

  //检查op是否继承自 Lockable
  static_assert(std::is_base_of<Lockable<T>, OP>::value, "OP must be derived from Lockable");

public:
  /**
   * @brief 构造函数，通过右值引用移动锁对象到UniqueLockAdapter
   *
   * @param lock 要移动的锁对象
   */
  UniqueLockAdapter(T &&lock) : m_lock(std::move(lock)) {

  }

  /**
   * @brief 删除拷贝构造函数以防止锁对象被拷贝
   */
  UniqueLockAdapter(const UniqueLockAdapter &) = delete;

  /**
   * @brief 删除拷贝赋值操作符以防止锁对象被拷贝
   */
  UniqueLockAdapter &operator=(const UniqueLockAdapter &) = delete;

  /**
   * @brief 默认移动构造函数，允许锁对象被移动
   */
  UniqueLockAdapter(UniqueLockAdapter &&) = default;

  /**
   * @brief 默认移动赋值操作符，允许锁对象被移动
   */
  UniqueLockAdapter &operator=(UniqueLockAdapter &&) = default;

  /**
   * @brief 默认构造函数，用于创建一个默认状态的锁对象
   */
  UniqueLockAdapter() = default;

  /**
   * @brief 虚析构函数，确保资源被正确释放
   *
   * 在析构函数中显式调用锁对象的析构函数，以处理可能的资源泄漏问题
   */
  virtual ~UniqueLockAdapter() {
	m_lock.~T();
  }

  /**
   * @brief 锁定锁对象
   *
   * 使用封装的锁操作对象的lock方法来锁定m_lock
   */
  void lock() {
	m_op.lock(m_lock);
  }

  /**
   * @brief 解锁锁对象
   *
   * 使用封装的锁操作对象的unlock方法来解锁m_lock
   */
  void unlock() {
	m_op.unlock(m_lock);
  }

  /**
   * @brief 尝试锁定锁对象，如果在指定时间内未能锁定则返回false
   *
   * @param t 尝试锁定的最大时间，单位毫秒
   * @return true 如果成功锁定，否则返回false
   */
  bool try_lock(unsigned long long t) {
	return m_op.try_lock(m_lock, t);
  }

  /**
   * @brief 检查锁对象是否已被锁定
   *
   * @return true 如果锁对象已被锁定，否则返回false
   */
  bool locked() CONST {
	return m_op.locked(m_lock);
  }

  /**
   * @brief 检查当前线程是否持有锁
   *
   * @return true 如果当前线程持有锁，否则返回false
   */
  bool heldByCurrentThread() CONST {
	return m_op.heldByCurrentThread(m_lock);
  }
};

namespace guard {

namespace functions {
template<typename T>
static inline void lock_on_init(T &t) {
  t.lock();
}

template<typename T>
static inline void unlock_on_destroy(T &t) {
  t.unlock();
}
}
template<typename T>
using auto_op_lock_guard = Guard<T, functions::lock_on_init<T>, functions::unlock_on_destroy<T>>;
}


}


#endif //TBS_TOOL_LIB_CONCURRENCY_INCLUDE_CONCURRENCY_LOCKADAPTER_H
