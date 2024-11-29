//
// Created by tongj on 24-11-29.
//

#ifndef CONSTEXPR_VALUE_H
#define CONSTEXPR_VALUE_H

#include <array>
#include "defs.h"
// 通用的编译时常量化结构体
template <typename T, size_t N>
struct ConstexprValue
{
    std::array<T, N> value;

    // constexpr 构造函数
    constexpr ConstexprValue() : value{}
    {
    }

    template <typename... Args>
    constexpr explicit ConstexprValue(Args... args) : value(args...)
    {
    }

    constexpr explicit ConstexprValue(CONST T* ptr) : value()
    {
        for (size_t i = 0; i < N; ++i)
        {
            value[i] = ptr[i];
        }
    }
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

    // constexpr 成员函数，返回值
    constexpr T& get(size_t index = 0)
    {
        return value[index];
    }

    // constexpr 成员函数，返回值（常量版本）
    constexpr const T& get(size_t index = 0) const
    {
        return value[index];
    }

    // constexpr 成员函数，返回数组大小
    [[nodiscard]] constexpr size_t size() const
    {
        return N;
    }
};

template <typename T>
struct ConstexprValue<T, 1>
{
    T value;
    // 默认构造函数，用于创建一个空的数组
    constexpr ConstexprValue() : value{}
    {
    }

    // 构造函数，用于创建一个包含指定值的数组
    template <typename... Args>
    constexpr explicit ConstexprValue(Args... args) : value(args...)
    {
    }

    template <typename U>
    constexpr bool operator==(const ConstexprValue<U, 1>& other) const
    {
        return value == other.value;
    }
    // 获取数组中的元素，始终返回默认值
    constexpr T& get()
    {
        return value;
    }
    [[nodiscard]] constexpr const T& get() const
    {
        return value;
    }
};


// 特化字符串类型
template <size_t N>
struct ConstexprValue<char, N>
{
    std::array<char, N> value;
    size_t length;


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
    // constexpr 构造函数
    constexpr explicit ConstexprValue(const char* str) : length(0)
    {
        while (str[length] != '\0' && length < N - 1)
        {
            value[length] = str[length];
            ++length;
        }
        value[length] = '\0'; // 确保字符串以 null 终止
    }

    // constexpr 成员函数，返回字符串长度
    [[nodiscard]] constexpr size_t size() const
    {
        return length;
    }

    // constexpr 成员函数，返回字符串的 C 风格表示
    [[nodiscard]] constexpr const char* c_str() const
    {
        return value.data();
    }

    // constexpr 成员函数，返回指定位置的字符
    constexpr char operator[](size_t index) const
    {
        return (index < length) ? value[index] : '\0';
    }
};

#define CONSTEXPR_STRING(name, str) constexpr ConstexprValue<char, sizeof(str)> name(str)


// 辅助函数，用于推导数组大小
template <typename T, size_t N>
constexpr ConstexprValue<T, N> make_ConstexprValue(const T (&arr)[N])
{
    return ConstexprValue<T, N>{arr};
}

template<typename T>
constexpr ConstexprValue<T,1> make_ConstexprValue(T&& value)
{
    return ConstexprValue<T,1>{value};
}


#endif //CONSTEXPR_VALUE_H
