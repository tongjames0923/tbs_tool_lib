//
// Created by abstergo on 24-6-12.
//

#ifndef TBS_CPP_TYPE_CHECK_H
#define TBS_CPP_TYPE_CHECK_H

#include <type_traits>

/**
 * @brief 是否包含针对指定类型的等价运算符 ，支持U.operator==(V)的类型
 * @tparam U 待检查类型
 * @tparam V 等价类型
 */
template<typename U, typename V> struct has_equal_operator {
  template<typename T, typename V1>
  static auto test(T *) -> decltype(std::declval<T>().operator==(std::declval<V1>()), std::true_type{});

  template<typename T, typename V1>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<U, V>(nullptr))::value;
};

/**
 * @brief 支持 T.operator==(T)
 */
template<typename T> using has_same_type_equal_operator = has_equal_operator<T, T>;

/**
 * @brief 判断类型中是否包含指定的函数操作符，支持 R(ARGS...) 的类型
 * @tparam U 判断的类型
 * @tparam R 函数返回值类型
 * @tparam ARGS 参数类型列表
 */
template<typename U, typename R, typename... ARGS>
struct has_method_operator {
public:
  // 使用 std::invoke_result_t 替代 std::result_of_t
  template<typename T>
  static auto test(T *) -> std::is_same<std::invoke_result_t<T, ARGS...>, R>;

  // 用于类型推导失败时的默认模板
  template<typename>
  static std::false_type test(...);

  // 使用 decltype 和 std::bool_constant 替代 std::is_same 的用法
  static constexpr bool value = decltype(test<U>(nullptr))::value;
};

/**
 * @brief 支持 void U(ARGS...)的类型
 */
template<typename T, typename ...ARGS> using has_none_return_method_operator = has_method_operator<T, void, ARGS...>;

template<typename T>
static std::string get_type_name() {
  return typeid(T).name();
}


// 基本情况，当参数包为空时终止递归

template<typename V, typename D>
void expandArgs(D d, V v) {
}

/**
 * @brief 展开函数参数包
 * @tparam T 当前函数参数包的参数
 * @tparam V 协调的参数类型
 * @tparam D 操作参数类型，必须有函数操作符bool()(V,T)来实现 bool返回true则终止匹配
 * @tparam Args 参数列表包
 * @param d
 * @param v
 * @param first
 * @param args
 */
template<typename T, typename V, typename D, typename ...Args>
inline void expandArgs(D d, V v, T first, Args... args) {

  static_assert(has_method_operator<D, bool, V, T>::value,
				"D must has method operator for T and return bool for break flag");
  bool break_flag = d.operator()(v, first);
  if (break_flag) {
	return;
  }
  // 递归处理剩余参数
  expandArgs(d, v, args...);
}

#endif //TBS_CPP_TYPE_CHECK_H
