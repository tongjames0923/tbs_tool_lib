//
// Created by tongj on 24-11-27.
//

#ifndef FORMAT_H
#define FORMAT_H

#include <format>
#include <tbs/defs.h>

/**
 * @brief 注册自定义格式化函数的宏
 *
 * 该宏用于定义并注册一个自定义的格式化函数，使得某个特定类型T可以使用std::format进行格式化输出。
 * 它通过特化std::formatter模板类，提供解析和格式化两个阶段的实现。
 *
 * @param T 要格式化的类型
 * @param parse_function 解析阶段的实现函数
 * @param format_function 格式化阶段的实现函数
 * @param members 特化类的成员变量或函数
 */
#define FORMATTER_REGISTER(T, parse_function, format_function, members)                                                                                                            \
    template <>                                                                                                                                                                    \
    struct std::formatter<T, char>                                                                                                                                                 \
    {                                                                                                                                                                              \
        members template <class ParseContext>                                                                                                                                      \
        constexpr ParseContext::iterator parse(ParseContext& _context)                                                                                                             \
        {                                                                                                                                                                          \
            parse_function;                                                                                                                                                        \
        }                                                                                                                                                                          \
                                                                                                                                                                                   \
        template <class FmtContext>                                                                                                                                                \
        FmtContext::iterator format(T& s, FmtContext& _context) const                                                                                                              \
        {                                                                                                                                                                          \
            format_function;                                                                                                                                                       \
        }                                                                                                                                                                          \
    };

/**
 * @brief 快速注册简单格式化函数的宏
 *
 * 该宏是FORMATTER_REGISTER的简化版本，用于快速注册那些不需要复杂解析和格式化的类型。
 * 它直接使用std::format_to函数进行格式化输出。
 *
 * @param T 要格式化的类型
 * @param str 格式化字符串
 * @param ... 可变参数，传递给格式化字符串
 */
#define FORMATTER_REGISTER_QUICK(T, str, ...) FORMATTER_REGISTER(T, return _context.begin(), return std::format_to(_context.out(), str, __VA_ARGS__), )

/**
 * @brief 格式化字符串函数
 *
 * 该函数使用std::format根据格式化字符串fmt和参数args...生成一个格式化后的字符串。
 *
 * @param fmt 格式化字符串
 * @param args 参数包，包含所有格式化所需参数
 * @return std::string 格式化后的字符串
 */
template <typename... Args>
std::string format_string(CONST char* fmt, Args&&... args)
{
    return std::vformat(fmt, std::make_format_args(args...));
}

/**
 * @brief 编译时常量表达式格式化函数
 *
 * 该函数是一个编译时常量表达式，用于在编译时根据格式化字符串fmt和参数args...生成一个格式化后的字符串。
 *
 * @param fmt 格式化字符串
 * @param args 参数包，包含所有格式化所需参数
 * @return std::string 格式化后的字符串
 */
template <typename... Args>
consteval std::string constexpr_format(CONST char* fmt, Args&&... args)
{
    return vformat(fmt, std::make_format_args(args...));
}


#endif // FORMAT_H
