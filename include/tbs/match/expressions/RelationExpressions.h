//
// Created by abstergo on 24-11-12.
//

#ifndef TBS_TOOL_LIB_INCLUDE_MATCH_EXPRESSIONS_RELATION_EXPRESSIONS_RELATIONEXPRESSIONS_H
#define TBS_TOOL_LIB_INCLUDE_MATCH_EXPRESSIONS_RELATION_EXPRESSIONS_RELATIONEXPRESSIONS_H

#include <tbs/defs.h>

/**
 * @brief 相等匹配表达式类
 *
 * 该类提供了一个模板方法test，用于测试两个参数是否相等。
 */
class EqualMatchExpression {
public:
  /**
   * @brief 测试两个参数是否相等
   *
   * @tparam T 目标参数的类型
   * @tparam O 其他参数的类型
   * @param target 目标参数
   * @param other 其他参数
   * @return true 如果两个参数相等，否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;
};


/**
 * NotEqualMatchExpression 类用于定义不等于比较的表达式
 * 它提供了一个模板函数来比较两个不同类型的值是否不相等
 */
class NotEqualMatchExpression {
  /**
   * 模板函数test用于判断两个值是否不相等
   * @param target 目标值
   * @param other 另一个值，用于与目标值进行比较
   * @return 如果目标值不等于另一个值，则返回true；否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;
};

/**
 * GreaterThanMatchExpression 类用于定义大于比较的表达式
 * 它提供了一个模板函数来判断第一个值是否大于第二个值
 */
class GreaterThanMatchExpression {
  /**
   * 模板函数test用于判断目标值是否大于另一个值
   * @param target 目标值
   * @param other 另一个值，用于与目标值进行比较
   * @return 如果目标值大于另一个值，则返回true；否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;
};

/**
 * GreaterThanOrEqualMatchExpression 类用于定义大于或等于比较的表达式
 * 它提供了一个模板函数来判断第一个值是否大于或等于第二个值
 */
class GreaterThanOrEqualMatchExpression {
  /**
   * 模板函数test用于判断目标值是否大于或等于另一个值
   * @param target 目标值
   * @param other 另一个值，用于与目标值进行比较
   * @return 如果目标值大于或等于另一个值，则返回true；否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;
};

/**
 * LessThanMatchExpression 类用于定义小于比较的表达式
 * 它提供了一个模板函数来判断第一个值是否小于第二个值
 */
class LessThanMatchExpression {
  /**
   * 模板函数test用于判断目标值是否小于另一个值
   * @param target 目标值
   * @param other 另一个值，用于与目标值进行比较
   * @return 如果目标值小于另一个值，则返回true；否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;
};

/**
 * LessThanOrEqualMatchExpression 类用于定义小于或等于比较的表达式
 * 它提供了一个模板函数来判断第一个值是否小于或等于第二个值
 */
class LessThanOrEqualMatchExpression {
  /**
   * 模板函数test用于判断目标值是否小于或等于另一个值
   * @param target 目标值
   * @param other 另一个值，用于与目标值进行比较
   * @return 如果目标值小于或等于另一个值，则返回true；否则返回false
   */
  template<typename T, typename O>
  bool test(T target, O other) CONST;
};


template<typename T, typename O>
bool GreaterThanOrEqualMatchExpression::test(T target, O other) CONST {
  return target > other || target == other;
}

template<typename T, typename O>
bool LessThanMatchExpression::test(T target, O other) CONST {
  return target < other;
}

template<typename T, typename O>
bool LessThanOrEqualMatchExpression::test(T target, O other) CONST {
  return target < other || target == other;
}

template<typename T, typename O>
bool NotEqualMatchExpression::test(T target, O other) CONST {
  return target != other;
}

template<typename T, typename O>
bool EqualMatchExpression::test(T target, O other) CONST {
  return target == other;
}


template<typename T, typename O>
bool GreaterThanMatchExpression::test(T target, O other) CONST {
  return target > other;
}

#endif //TBS_TOOL_LIB_INCLUDE_MATCH_EXPRESSIONS_RELATION_EXPRESSIONS_RELATIONEXPRESSIONS_H
