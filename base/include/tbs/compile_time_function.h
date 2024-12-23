//
// Created by tongj on 24-11-29.
//

#ifndef COMPILE_TIME_FUNCTION_H
#define COMPILE_TIME_FUNCTION_H

#include <optional>
#include "type_check.h"

namespace
{
    /**
     * 在编译时执行给定的函数对象
     *
     * @tparam Func 函数对象类型
     * @return 函数的返回值，如果函数返回 void，则返回一个空的 optional
     */
    template <typename Func>
    constexpr std::optional<InvokeResultType<Func>> executeFunc()
    {
        if constexpr (isVoidReturn<Func>())
        {
            Func{}();
            return std::optional<InvokeResultType<Func>>();
        }
        else
        {
            return Func{}();
        }
    }
} // namespace

/**
 * 编译时条件选择结构体
 *
 * @tparam Condition 条件表达式
 * @tparam TrueType 条件为真时的类型
 * @tparam FalseType 条件为假时的类型
 */
template <bool Condition, typename TrueType, typename FalseType>
struct CompileTimeIf;

/**
 * 条件为真时的特化结构体
 *
 * @tparam TrueType 条件为真时的类型
 * @tparam FalseType 条件为假时的类型
 */
template <typename TrueType, typename FalseType>
struct CompileTimeIf<true, TrueType, FalseType>
{
    /**
     * 目标类型，即条件为真时的类型
     */
    using TargetType = TrueType;

    /**
     * 返回类型，即目标类型的返回类型
     */
    using ReturnType = InvokeResultType<TargetType>;

    static consteval bool isValid()
    {
        return true;
    }

    constexpr CompileTimeIf() = default;
    static consteval CompileTimeIf value()
    {
        return CompileTimeIf();
    }

    /**
     * 在编译时执行目标类型
     *
     * @return 目标类型的返回值，如果目标类型返回 void，则返回一个空的 optional
     */
    constexpr std::optional<ReturnType> execute()
    {
        return executeFunc<TargetType>();
    }
};

/**
 * 条件为假时的特化结构体
 *
 * @tparam TrueType 条件为真时的类型
 * @tparam FalseType 条件为假时的类型
 */
template <typename TrueType, typename FalseType>
struct CompileTimeIf<false, TrueType, FalseType>
{
    /**
     * 目标类型，即条件为真时的类型
     */
    using TargetType = FalseType;

    /**
     * 返回类型，即目标类型的返回类型
     */
    using ReturnType = InvokeResultType<TargetType>;

    static consteval bool isValid()
    {
        return false;
    }

    constexpr CompileTimeIf() = default;
    static consteval CompileTimeIf value()
    {
        return CompileTimeIf();
    }

    /**
     * 在编译时执行目标类型
     *
     * @return 目标类型的返回值，如果目标类型返回 void，则返回一个空的 optional
     */
    constexpr std::optional<ReturnType> execute()
    {
        return executeFunc<TargetType>();
    }
};

/**
 * 编译时条件选择宏
 *
 * @param condition 条件表达式
 * @param true_expr 条件为真时的表达式
 * @param false_expr 条件为假时的表达式
 * @return 条件为真时执行 true_expr，条件为假时执行 false_expr
 */
#define COMPILE_TIME_IF_TYPE(condition, true_expr, false_expr) CompileTimeIf<condition, decltype(true_expr), decltype(false_expr)>

/**
 * 定义一个编译时条件选择宏。
 * 该宏在编译时根据给定的条件选择不同的表达式。
 *
 * @param condition 条件表达式，用于判断是否满足条件
 * @param true_expr 当条件为真时执行的表达式
 * @param false_expr 当条件为假时执行的表达式
 * @return 返回选择的表达式的值
 */
#define COMPILE_TIME_IF_BODY(condition, true_expr, false_expr) COMPILE_TIME_IF_TYPE(condition, true_expr, false_expr)::value()

/**
 * 定义一个编译时条件选择结果宏。
 * 该宏用于获取编译时条件选择的结果。
 *
 * @param condition 条件表达式，用于判断是否满足条件
 * @param true_expr 当条件为真时执行的表达式
 * @param false_expr 当条件为假时执行的表达式
 * @return 返回最终选择的表达式的执行结果
 */
#define COMPILE_TIME_IF_RESULT(condition, true_expr, false_expr) COMPILE_TIME_IF_BODY(condition, true_expr, false_expr).execute()

#endif // COMPILE_TIME_FUNCTION_H
