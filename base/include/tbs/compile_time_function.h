//
// Created by tongj on 24-11-29.
//

#ifndef COMPILE_TIME_FUNCTION_H
#define COMPILE_TIME_FUNCTION_H
#include <optional>
#include "type_check.h"

namespace
{
    template <typename Func>
    constexpr std::optional<InvokeResultType<Func>> executeFunc()
    {
        {
            if constexpr (isVoidReturn<Func>())
            {
                Func{}();
                return std::optional<InvokeResultType<Func>>();
            }
            else
                return Func{}();
        }
    }
} // namespace

template <bool Condition, typename TrueType, typename FalseType>
struct CompileTimeIf;

template <typename TrueType, typename FalseType>
struct CompileTimeIf<true, TrueType, FalseType>
{
    using TargetType = TrueType;
    using ReturnType = InvokeResultType<TargetType>;
    consteval static std::optional<ReturnType> execute()
    {
        return executeFunc<TargetType>();
    }
};

template <typename TrueType, typename FalseType>
struct CompileTimeIf<false, TrueType, FalseType>
{
    using TargetType = FalseType;
    using ReturnType = InvokeResultType<TargetType>;
    consteval static std::optional<ReturnType> execute()
    {
        return executeFunc<TargetType>();
    }
};


#define COMPILE_TIME_IF(condition, true_expr, false_expr) CompileTimeIf<condition, decltype(true_expr), decltype(false_expr)>::execute()

#endif // COMPILE_TIME_FUNCTION_H
