//
// Created by abstergo on 24-11-14.
//

#ifndef TBS_TOOL_LIB_BASE_INCLUDE_GUARD_H
#define TBS_TOOL_LIB_BASE_INCLUDE_GUARD_H

#include <defs.h>
#include <functional>


template<typename T>
using guard_operator = void (*)(T &);

template<typename T>
inline static void delete_ptr_destruction(T &ptr) {
  delete ptr;
  ptr = nullptr;
}

template<typename T>
inline static void delete_array_destruction(T &ptr) {
  delete[] ptr;
  ptr = nullptr;
}

template<typename T>
inline static void no_action(T &ref) {

}


template<typename T, guard_operator<T> __INIT, guard_operator<T> __DESTRUCTION>
class Guard {
  T &m_ref;
public:
  explicit Guard(T &ref) : m_ref(ref) {
	__INIT(m_ref);
  }

  explicit Guard(T &&ref) : m_ref(ref) {
	__INIT(m_ref);
  }


  ~Guard() {
	__DESTRUCTION(m_ref);
  }
};


template<typename T>
using ptr_guard = Guard<T, no_action<T>, delete_ptr_destruction<T>>;

template<typename T>
using array_guard = Guard<T, no_action<T>, delete_array_destruction<T>>;

#endif //TBS_TOOL_LIB_BASE_INCLUDE_GUARD_H
