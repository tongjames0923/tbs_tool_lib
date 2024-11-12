//
// Created by abstergo on 24-11-12.
//

#ifndef TBS_TOOL_LIB_INCLUDE_MATCH_MATCHER_H
#define TBS_TOOL_LIB_INCLUDE_MATCH_MATCHER_H

#include <defs.h>
#include <type_check.h>

/c++
/**
 * @brief Matcher类模板
 *
 * 用于定义一个匹配器对象，该对象可以验证一个给定的值是否与内部存储的值相等，
 * 并且可以通过一个用户定义的函数对值进行处理。这个类模板允许灵活地定义匹配和处理逻辑。
 *
 * @tparam V 值的类型
 * @tparam E 等值比较函数对象的类型
 * @tparam F 用户定义的处理函数对象的类型
 */
template<typename V, typename E, typename F>
class Matcher {
private:
  V _value; // 内部存储的值
  E _equals; // 等值比较函数对象
  F _f; // 用户定义的处理函数对象

public:
  /**
   * @brief 构造函数
   *
   * 通过右值引用移动构造Matcher对象。
   *
   * @param value 要存储的值，通过右值引用传递
   * @param f 用户定义的处理函数对象
   */
  Matcher(V &&value, F f)
	  : _value(value), _f(f) {

  }

  /**
   * @brief 构造函数
   *
   * 通过常量引用构造Matcher对象。
   *
   * @param v 要存储的值，通过常量引用传递
   * @param f 用户定义的处理函数对象
   */
  Matcher(const V &v, F f) : _value(v), _f(f) {

  }

  /**
   * @brief 构造函数
   *
   * 通过常量引用和等值比较函数对象构造Matcher对象。
   *
   * @param value 要存储的值，通过常量引用传递
   * @param equals 等值比较函数对象
   * @param f 用户定义的处理函数对象
   */
  Matcher(const V &value, E equals, F f)
	  : _value(value), _equals(equals), _f(f) {

  }

  /**
   * @brief 构造函数
   *
   * 通过右值引用和等值比较函数对象构造Matcher对象。
   *
   * @param value 要存储的值，通过右值引用传递
   * @param equals 等值比较函数对象
   * @param f 用户定义的处理函数对象
   */
  Matcher(V &&value, E equals, F f)
	  : _value(value), _equals(equals), _f(f) {

  }

  /**
   * @brief 构造函数
   *
   * 仅通过用户定义的处理函数对象构造Matcher对象。
   *
   * @param f 用户定义的处理函数对象
   */
  explicit Matcher(F f)
	  : _f(f) {

  }

  /**
   * @brief 测试函数
   *
   * 使用等值比较函数对象测试给定的值是否与内部存储的值相等。
   *
   * @tparam T 给定值的类型
   * @param t 给定的值
   * @return true 如果给定的值与内部存储的值相等，否则返回false
   */
  template<typename T>
  bool test(T t) const {
	return _equals.test(_value, t);
  }

  /**
   * @brief 处理函数
   *
   * 使用用户定义的处理函数对象处理给定的值。
   *
   * @tparam T 给定值的类型
   * @param t 给定的值
   * @return auto 处理结果，类型由用户定义的处理函数决定
   */
  template<typename T>
  auto operator()(T t) const {
	return _f(t,_value);
  }

};

#endif //TBS_TOOL_LIB_INCLUDE_MATCH_MATCHER_H
