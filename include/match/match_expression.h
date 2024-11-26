//
// Created by abstergo on 24-11-12.
//

#ifndef TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_EXPRESSION_H
#define TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_EXPRESSION_H

#include <tbs/match/expressions/RelationExpressions.h>

/**
 * @brief 任意匹配表达式类
 *
 * 该类提供了一个模板方法test，用于测试两个参数是否在某种条件下匹配。
 * 它可以与任何定义了test方法的其他类一起使用。
 */
class AnyMatchExpression {
public:
  /**
   * @brief 测试两个参数是否匹配
   *
   * @tparam T 目标参数的类型
   * @tparam O 其他参数的类型
   * @param target 目标参数
   * @param other 其他参数
   * @return true 如果两个参数匹配，否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;
};


/**
 * @brief 函数式匹配表达式类
 *
 * 该类提供了一个模板方法test，用于通过一个函数对象来测试两个参数是否匹配。
 * 它允许用户定义复杂的匹配逻辑。
 *
 * @tparam E 函数对象的类型
 */
template<typename E>
class FunctionalMatchExpression {
public:
  /**
   * @brief 测试两个参数是否匹配
   *
   * 使用成员函数对象_func来执行匹配逻辑。
   *
   * @tparam T 目标参数的类型
   * @tparam O 其他参数的类型
   * @param target 目标参数
   * @param other 其他参数
   * @return true 如果两个参数根据_func定义的逻辑匹配，否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;

  /**
   * @brief 构造函数
   *
   * 初始化函数对象_func。
   *
   * @param func 函数对象，用于定义匹配逻辑
   */
  explicit FunctionalMatchExpression(E func) : _func(func) {
  }

private:
  E _func; // 函数对象，用于定义匹配逻辑
};


template<typename E>
template<typename T, typename O>
bool FunctionalMatchExpression<E>::test(T target, O other) CONST {
  return _func();
}

template<typename T, typename O>
bool AnyMatchExpression::test(T target, O other) CONST {
  return true;
}


#endif //TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_EXPRESSION_H
