//
// Created by abstergo on 24-11-12.
//

#ifndef TBS_TOOL_LIB_INCLUDE_MATCH_MATCHERS_H
#define TBS_TOOL_LIB_INCLUDE_MATCH_MATCHERS_H

#include <tbs/match/matcher.h>
#include <tbs/match/match_expression.h>

// 定义一个类型别名模板EqualableMatcher，用于匹配相等的值
// T表示待比较的类型，F表示比较函数的类型
template<typename T, typename F>
using EqualableMatcher = Matcher<T, EqualMatchExpression, F>;

// 定义一个类型别名模板NotEqualMatcher，用于匹配不相等的值
// T表示待比较的类型，F表示比较函数的类型
template<typename T, typename F>
using NotEqualMatcher = Matcher<T, NotEqualMatchExpression, F>;

// 定义一个模板别名GreaterThanMatcher，用于匹配大于操作
template<typename T, typename F>
using GreaterThanMatcher = Matcher<T, GreaterThanMatchExpression, F>;

// 定义一个模板别名GreaterThanOrEqualMatcher，用于匹配大于等于操作
template<typename T, typename F>
using GreaterThanOrEqualMatcher = Matcher<T, GreaterThanOrEqualMatchExpression, F>;

// 定义一个模板别名LessThanMatcher，用于匹配小于操作
template<typename T, typename F>
using LessThanMatcher = Matcher<T, LessThanMatchExpression, F>;

// 定义一个模板别名LessThanOrEqualMatcher，用于匹配小于等于操作
template<typename T, typename F>
using LessThanOrEqualMatcher = Matcher<T, LessThanOrEqualMatchExpression, F>;



// 定义一个类型别名模板AnyMatcher，用于匹配任何值
// F表示匹配函数的类型
template<typename F>
using AnyMatcher = Matcher<void *, AnyMatchExpression, F>;

// 定义一个类型别名模板FunctionalMatcher，用于通过函数进行匹配
// F表示匹配函数的类型，E表示函数表达式的类型
template<typename F, typename E>
using FunctionalMatcher = Matcher<void *, FunctionalMatchExpression<E>, F>;

// 创建一个AnyMatcher对象，用于匹配任何类型的值
// 参数f是一个可调用对象，用于执行匹配操作
template<typename F>
AnyMatcher<F> make_any_matcher(F &&f) {
    return AnyMatcher<F>(nullptr, f);
}

// 创建一个EqualableMatcher对象，用于匹配相等的值
// 参数t是用于比较的值，f是一个可调用对象，用于执行匹配操作
template<typename T, typename F>
EqualableMatcher<T, F> make_equal_matcher(T &&t, F &&f) {
    return EqualableMatcher<T, F>(std::move(t), f);
}

// 创建一个FunctionalMatcher对象，用于通过函数进行匹配
// 参数f是一个可调用对象，e是一个表达式对象，用于执行匹配操作
template<typename F, typename E>
FunctionalMatcher<F, E> make_functional_matcher(F &&f, E &&e) {
    return FunctionalMatcher<F, E>(nullptr, (FunctionalMatchExpression<E>) e, f);
}


#endif //TBS_TOOL_LIB_INCLUDE_MATCH_MATCHERS_H
