//
// Created by abstergo on 24-11-12.
//

#ifndef TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_MACRO_H
#define TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_MACRO_H


// 定义一个宏，用于创建一个可以匹配任何值的匹配器
#define ANY_MATCH(f) make_any_matcher([&](auto _target_, auto _v_) -> auto { f })

// 定义一个宏，用于创建一个可以匹配特定值的匹配器
#define EQUAL_MATCH(val, f) make_equal_matcher(val, [&](auto _target_, auto _v_) -> auto { f })

// 定义一个宏，用于创建一个基于函数的匹配器
#define FUNCTION_MATCH(e, f) make_functional_matcher([&](auto _target_, auto _v_) -> auto { f }, [&]() { return e; })

// 定义一个模板函数，用于将一个值与一个匹配器进行匹配
template<typename T, typename F>
auto match(T v, F f)
{
    // 如果匹配器测试通过，则调用匹配器并返回结果
    if (f.test(v))
    {
        return f(v);
    }
    // 否则，直接返回输入值
    return v;
}

// 定义一个模板函数，用于将一个值与一个匹配器进行匹配
template<typename T, typename F>
void matchWithoutReturn(T v, F f)
{
    // 如果匹配器测试通过，则调用匹配器并返回结果
    if (f.test(v))
    {
        f(v);
    }
}

// 定义一个模板函数，用于将一个值与多个匹配器进行匹配
template<typename T, typename F, typename... ARGS>
auto match(T v, F f, ARGS... args)
{
    // 如果匹配器测试通过，则调用匹配器并返回结果
    if (f.test(v))
    {
        return f(v);
    }
    // 否则，继续尝试与剩余的匹配器进行匹配
    return match(v, args...);
}

// 定义一个模板函数，用于将一个值与多个匹配器进行匹配
template<typename T, typename F, typename... ARGS>
void matchWithoutReturn(T v, F f, ARGS... args)
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

// 定义一个模板函数，用于将一个值与多个匹配器进行匹配
template<typename T, typename... ARGS>
auto match(T v, ARGS... args)
{
    // 调用上述函数，尝试与所有匹配器进行匹配
    return match(v, args...);
}

// 定义一个模板函数，用于将一个值与多个匹配器进行匹配
template<typename T, typename... ARGS>
void matchWithoutReturn(T v, ARGS... args)
{
    // 调用上述函数，尝试与所有匹配器进行匹配
    matchWithoutReturn(v, args...);
}

#endif // TBS_TOOL_LIB_INCLUDE_MATCH_MATCH_MACRO_H
