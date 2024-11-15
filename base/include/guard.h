//
// Created by abstergo on 24-11-14.
//

#ifndef TBS_TOOL_LIB_BASE_INCLUDE_GUARD_H
#define TBS_TOOL_LIB_BASE_INCLUDE_GUARD_H

#include <defs.h>
#include <functional>
#include <optional>
// 定义 guard_operator 类型
template<typename T>
using guard_operator = void (*)(T &);

// 初始化操作
template<typename T>
constexpr void delete_ptr_destruction(T &ptr) noexcept {
  delete ptr;
  ptr = nullptr;
}

// 数组初始化操作
template<typename T>
constexpr void delete_array_destruction(T &ptr) noexcept {
  delete[] ptr;
  ptr = nullptr;
}

// Guard 类
template<typename T, guard_operator<T> __INIT, guard_operator<T> __DESTRUCTION>
class Guard {
private:
  using __item_type = T *;
  __item_type m_ref;

  // 初始化函数
  constexpr void init() {
	if constexpr (__INIT != nullptr) {
	  if (m_ref != nullptr) {
		__INIT(*m_ref);
	  }
	}
  }

public:

  // 构造函数
  explicit Guard(T &ref) NO_EXCEPT: m_ref(&ref) {
	init();
  }

  explicit Guard(T &&ref) NO_EXCEPT: m_ref(&ref) {
	init();
  }

  Guard(Guard &&ref) noexcept: m_ref(ref.m_ref) {
	ref.m_ref = nullptr;
  }

  Guard(const Guard &ref) = delete;

  Guard &operator=(const Guard &ref) = delete;

  Guard &operator=(Guard &&ref) noexcept {
	m_ref = ref.m_ref;
	ref.m_ref = nullptr;
	return *this;
  }


  T &operator*() noexcept {
	return *m_ref;
  }


  // 析构函数
  ~Guard() noexcept {
	if constexpr (__DESTRUCTION != nullptr) {
	  if (m_ref != nullptr) {
		__DESTRUCTION(*m_ref);
	  }
	}
  }
};

// 指针 Guard
template<typename T>
using ptr_guard = Guard<T, nullptr, delete_ptr_destruction<T>>;

// 数组 Guard
template<typename T>
using array_guard = Guard<T, nullptr, delete_array_destruction<T>>;

#endif //TBS_TOOL_LIB_BASE_INCLUDE_GUARD_H
