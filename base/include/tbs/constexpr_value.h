//
// Created by tongj on 24-11-29.
//

#ifndef CONSTEXPR_VALUE_H
#define CONSTEXPR_VALUE_H

#include <array>
#include "defs.h"

/**
 * 通用的编译时常量化结构体
 *
 * @tparam T 元素类型
 * @tparam N 数组大小
 */
template <typename T, size_t N>
struct ConstexprValue
{
    std::array<T, N> value;

    /**
     * 默认构造函数，初始化为空数组
     */
    constexpr ConstexprValue() : value{}
    {
    }

    /**
     * 从指针初始化数组
     *
     * @param ptr 指向初始值的指针
     */
    constexpr explicit ConstexprValue(CONST T* ptr) : value()
    {
        for (size_t i = 0; i < N; ++i)
        {
            value[i] = ptr[i];
        }
    }

    /**
     * 比较两个 ConstexprValue 是否相等
     *
     * @param other 要比较的对象
     * @return 如果相等返回 true，否则返回 false
     */
    template <typename U, size_t C>
    constexpr bool operator==(const ConstexprValue<U, C>& other) const
    {
        static_assert(N == C, "ConstexprValue size mismatch");
        for (size_t i = 0; i < N; ++i)
        {
            if (value[i] != other.value[i])
            {
                return false;
            }
        }
        return true;
    }

    /**
     * 获取数组中的元素
     *
     * @param index 元素索引，默认为 0
     * @return 引用数组中的元素
     */
    constexpr T& get(size_t index = 0)
    {
        return value[index];
    }

    /**
     * 获取数组中的元素（常量版本）
     *
     * @param index 元素索引，默认为 0
     * @return 常量引用数组中的元素
     */
    constexpr const T& get(size_t index = 0) const
    {
        return value[index];
    }

    /**
     * 返回数组的大小
     *
     * @return 数组的大小
     */
    [[nodiscard]] constexpr size_t size() const
    {
        return N;
    }
};

/**
 * 专门用于单个元素的 ConstexprValue 结构体
 *
 * @tparam T 元素类型
 */
template <typename T>
struct ConstexprValue<T, 1>
{
    T value;

    /**
     * 默认构造函数，初始化为空值
     */
    constexpr ConstexprValue() : value{}
    {
    }

    /**
     * 带参数的构造函数，初始化单个元素
     *
     * @param args 可变参数列表，用于初始化单个元素
     */
    template <typename... Args>
    constexpr explicit ConstexprValue(Args... args) : value(args...)
    {
    }

    /**
     * 比较两个 ConstexprValue 是否相等
     *
     * @param other 要比较的对象
     * @return 如果相等返回 true，否则返回 false
     */
    template <typename U>
    constexpr bool operator==(const ConstexprValue<U, 1>& other) const
    {
        return value == other.value;
    }

    /**
     * 获取单个元素
     *
     * @return 引用单个元素
     */
    constexpr T& get()
    {
        return value;
    }

    /**
     * 获取单个元素（常量版本）
     *
     * @return 常量引用单个元素
     */
    [[nodiscard]] constexpr const T& get() const
    {
        return value;
    }
};

/**
 * 专门用于字符数组的 ConstexprValue 结构体
 *
 * @tparam N 字符数组的大小
 */
template <size_t N>
struct ConstexprValue<char, N>
{
    std::array<char, N> value;
    size_t length;

    /**
     * 比较两个 ConstexprValue 是否相等
     *
     * @param other 要比较的对象
     * @return 如果相等返回 true，否则返回 false
     */
    template <typename U, size_t C>
    constexpr bool operator==(const ConstexprValue<U, C>& other) const
    {
        for (size_t i = 0; i < N; ++i)
        {
            if (value[i] != other.value[i])
            {
                return false;
            }
        }
        return true;
    }

    /**
     * 从 C 风格字符串初始化字符数组
     *
     * @param str C 风格字符串
     */
    constexpr explicit ConstexprValue(const char* str) : length(0), value{}
    {
        while (str[length] != '\0' && length <= N - 1)
        {
            value[length] = str[length];
            ++length;
        }
        value[length] = '\0'; // 确保字符串以 null 终止
    }

    template <size_t N1>
    explicit constexpr ConstexprValue(CONST ConstexprValue<char, N1>& other) : length(0), value{}
    {
        auto str = other.c_str();
        while (*str != '\0')
        {
            value[length] = *str;
            ++length;
            ++str;
        }
        value[length] = '\0';
    }
    template <size_t N1>
    constexpr ConstexprValue<char, N1> operator=(CONST ConstexprValue<char, N1>& other) CONST
    {
        ConstexprValue<char, N1> result("");
        auto str = other.c_str();
        while (*str != '\0')
        {
            result.value[result.length] = *str;
            ++(result.length);
            ++str;
        }
        result.value[result.length] = '\0';
        return result;
    }

    template <size_t N1>
    constexpr ConstexprValue<char, N + N1> operator+(CONST ConstexprValue<char, N1>& other) const
    {
        ConstexprValue<char, N + N1> result("");
        auto ptr = this->c_str();
        while (*ptr != '\0')
        {
            result.value[result.length] = *ptr;
            ++(result.length);
            ++ptr;
        }
        ptr = other.c_str();
        while (*ptr != '\0')
        {
            result.value[result.length] = *ptr;
            ++(result.length);
            ++ptr;
        }
        result.value[result.length] = '\0';
        return result;
    }

    /**
     * 返回字符串的长度
     *
     * @return 字符串的长度
     */
    [[nodiscard]] constexpr size_t size() const
    {
        return length;
    }

    /**
     * 返回字符串的 C 风格表示
     *
     * @return 字符串的 C 风格表示
     */
    [[nodiscard]] constexpr const char* c_str() const
    {
        return value.data();
    }

    /**
     * 获取指定位置的字符
     *
     * @param index 字符索引
     * @return 指定位置的字符，如果索引超出范围则返回 '\0'
     */
    constexpr char operator[](size_t index) const
    {
        return (index < length) ? value[index] : '\0';
    }
};

/**
 * 常量字符串结构体，继承自 ConstexprValue
 *
 * @tparam N 字符串的长度
 */

template <size_t N>
using ConstexprString = ConstexprValue<char, N>;
/**
 * 定义一个常量字符串
 *
 * @param name 常量字符串的名称
 * @param str 字符串字面量
 */
#define CONSTEXPR_STRING_NAMED(name, str) ConstexprString<sizeof(str)> name(str)

/**
 * 定义一个常量字符串
 *
 * @param str 字符串字面量
 */
#define CONSTEXPR_STRING(str) CONSTEXPR_STRING_NAMED(, str)

/**
 * 辅助函数，用于从数组创建 ConstexprValue 对象
 *
 * @param arr 要转换的数组
 * @return 创建的 ConstexprValue 对象
 */
template <typename T, size_t N>
constexpr ConstexprValue<T, N> make_ConstexprValue(const T (&arr)[N])
{
    return ConstexprValue<T, N>{arr};
}

/**
 * 辅助函数，用于从单个值创建 ConstexprValue 对象
 *
 * @param value 要转换的值
 * @return 创建的 ConstexprValue 对象
 */
template<typename T>
constexpr ConstexprValue<T,1> make_ConstexprValue(T&& value)
{
    return ConstexprValue<T,1>{value};
}

#endif //CONSTEXPR_VALUE_H
