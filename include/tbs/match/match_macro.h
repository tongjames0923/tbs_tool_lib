//
// Created by abstergo on 24-11-12.
//

#ifndef TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_MACRO_H
#define TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_MACRO_H

#include "matchers.h"

/**
 * @brief 定义一个宏，用于创建一个可以匹配任何值的匹配器。
 *
 * @param f 匹配逻辑的 lambda 表达式
 */
#define ANY_MATCH(f) make_any_matcher([&](const auto& _target_, const auto& _v_) -> auto { f })

/**
 * @brief 定义一个宏，用于创建一个可以匹配特定值的匹配器。
 *
 * @param val 要匹配的目标值
 * @param f 匹配逻辑的 lambda 表达式
 */
#define EQUAL_MATCH(val, f) make_equal_matcher(val, [&](const auto& _target_, const auto& _v_) -> auto { f })

/**
 * @brief 定义一个宏，用于创建一个基于函数的匹配器。
 *
 * @param e 函数表达式的返回值
 * @param f 匹配逻辑的 lambda 表达式
 */
#define FUNCTION_MATCH(e, f) make_functional_matcher([&](const auto& _target_, const auto& _v_) -> auto { f }, [&]() { return e; })

/**
 * @brief 定义一个宏，用于创建一个不返回值的匹配逻辑。
 *
 * @param val 要匹配的值
 * @param cases 匹配逻辑的 case 列表
 * @param _else 默认情况下执行的逻辑
 */
#define NO_RETURNED_MATCH(val, cases, _else) matchWithoutReturn(val, cases, []() { _else })

/**
 * @brief 定义一个宏，用于创建一个返回值的匹配逻辑。
 *
 * @param val 要匹配的值
 * @param cases 匹配逻辑的 case 列表
 * @param _else 默认情况下执行的逻辑
 */
#define RETURNED_MATCH(val, cases, _else) match(val, cases, []() { _else })

/**
 * @brief 定义一个模板函数，用于将一个值与一个匹配器进行匹配。
 *
 * @tparam T 输入值的类型
 * @tparam F 匹配器的类型
 * @tparam E 默认逻辑的类型
 * @param v 输入值
 * @param f 匹配器
 * @param e 默认逻辑
 * @return 如果匹配成功则返回匹配结果，否则返回默认逻辑的结果
 */
template <typename T, typename F, typename E>
auto match(CONST T& v, F&& f, E&& e)
{
    // 如果匹配器测试通过，则调用匹配器并返回结果
    if (f.test(v))
    {
        return f(v);
    }
    // 否则，直接返回输入值
    return e();
}

/**
 * @brief 定义一个模板函数，用于将一个值与一个匹配器进行匹配（不返回值）。
 *
 * @tparam T 输入值的类型
 * @tparam F 匹配器的类型
 * @tparam E 默认逻辑的类型
 * @param v 输入值
 * @param f 匹配器
 * @param e 默认逻辑
 */
template <typename T, typename F, typename E>
void matchWithoutReturn(CONST T& v, F&& f, E&& e)
{
    // 如果匹配器测试通过，则调用匹配器并返回结果
    if (f.test(v))
    {
        f(v);
    }
    e();
}

/**
 * @brief 定义一个模板函数，用于将一个值与多个匹配器进行匹配。
 *
 * @tparam T 输入值的类型
 * @tparam F 第一个匹配器的类型
 * @tparam ARGS 其他匹配器的类型列表
 * @param v 输入值
 * @param f 第一个匹配器
 * @param args 其他匹配器
 * @return 如果匹配成功则返回匹配结果，否则继续尝试其他匹配器
 */
template <typename T, typename F, typename... ARGS>
auto match(CONST T& v, F&& f, ARGS... args)
{
    // 如果匹配器测试通过，则调用匹配器并返回结果
    if (f.test(v))
    {
        return f(v);
    }
    // 否则，继续尝试与剩余的匹配器进行匹配
    return match(v, args...);
}

/**
 * @brief 定义一个模板函数，用于将一个值与多个匹配器进行匹配。
 *
 * @tparam T 输入值的类型
 * @tparam ARGS 匹配器的类型列表
 * @param v 输入值
 * @param args 匹配器列表
 * @return 如果匹配成功则返回匹配结果，否则继续尝试其他匹配器
 */
template <typename T, typename... ARGS>
auto match(CONST T& v, ARGS... args)
{
    // 调用上述函数，尝试与所有匹配器进行匹配
    return match(v, args...);
}

/**
 * @brief 定义一个模板函数，用于将一个值与多个匹配器进行匹配（不返回值）。
 *
 * @tparam T 输入值的类型
 * @tparam F 第一个匹配器的类型
 * @tparam ARGS 其他匹配器的类型列表
 * @param v 输入值
 * @param f 第一个匹配器
 * @param args 其他匹配器
 */
template <typename T, typename F, typename... ARGS>
void matchWithoutReturn(CONST T& v, F&& f, ARGS... args)
{
    // 如果匹配器测试通过，则调用匹配器并返回结果
    if (f.test(v))
    {
        f(v);
        return;
    }
    // 否则，继续尝试与剩余的匹配器进行匹配
    matchWithoutReturn(v, args...);
}

/**
 * @brief 定义一个模板函数，用于将一个值与多个匹配器进行匹配（不返回值）。
 *
 * @tparam T 输入值的类型
 * @tparam ARGS 匹配器的类型列表
 * @param v 输入值
 * @param args 匹配器列表
 */
template <typename T, typename... ARGS>
void matchWithoutReturn(CONST T& v, ARGS... args)
{
    // 调用上述函数，尝试与所有匹配器进行匹配
    matchWithoutReturn(v, args...);
}

#endif // TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_MACRO_H
