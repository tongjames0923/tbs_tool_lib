//
// Created by tongj on 24-11-29.
//

#ifndef CONSTEXPR_VALUE_H
#define CONSTEXPR_VALUE_H

#include <array>
#include "defs.h"

/**
 * @brief 通用的编译时常量化结构体
 *
 * @tparam T 元素类型
 * @tparam N 数组大小
 */
template <typename T, size_t N>
struct ConstexprValue
{
    std::array<T, N> value;

    /**
     * @brief 默认构造函数，初始化为空数组
     */
    constexpr ConstexprValue() : value{}
    {
    }

    /**
     * @brief 从指针初始化数组
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
     * @brief 比较两个 ConstexprValue 是否相等
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
     * @brief 获取数组中的元素
     *
     * @param index 元素索引，默认为 0
     * @return 引用数组中的元素
     */
    constexpr T& get(size_t index = 0)
    {
        return value[index];
    }

    /**
     * @brief 获取数组中的元素（常量版本）
     *
     * @param index 元素索引，默认为 0
     * @return 常量引用数组中的元素
     */
    constexpr const T& get(size_t index = 0) const
    {
        return value[index];
    }

    /**
     * @brief 返回数组的大小
     *
     * @return 数组的大小
     */
    [[nodiscard]] constexpr size_t size() const
    {
        return N;
    }
};

/**
 * @brief 专门用于单个元素的 ConstexprValue 结构体
 *
 * @tparam T 元素类型
 */
template <typename T>
struct ConstexprValue<T, 1>
{
    T value;

    /**
     * @brief 默认构造函数，初始化为空值
     */
    constexpr ConstexprValue() : value{}
    {
    }

    /**
     * @brief 带参数的构造函数，初始化单个元素
     *
     * @param args 可变参数列表，用于初始化单个元素
     */
    template <typename... Args>
    constexpr explicit ConstexprValue(Args... args) : value(args...)
    {
    }

    /**
     * @brief 比较两个 ConstexprValue 是否相等
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
     * @brief 获取单个元素
     *
     * @return 引用单个元素
     */
    constexpr T& get()
    {
        return value;
    }

    /**
     * @brief 获取单个元素（常量版本）
     *
     * @return 常量引用单个元素
     */
    [[nodiscard]] constexpr const T& get() const
    {
        return value;
    }
};

#include <string>
#include <string_view>

/**
 * @brief 专门用于字符数组的 ConstexprValue 结构体
 *
 * @tparam N 字符数组的大小
 */
template <size_t N>
struct ConstexprValue<char, N>
{
    std::array<char, N> value;
    size_t length;

    /**
     * @brief 比较两个 ConstexprValue 是否相等
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
     * @brief 从 C 风格字符串初始化字符数组
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

    /**
     * @brief 从 std::string_view 初始化字符数组
     *
     * @param sv std::string_view
     */
    constexpr explicit ConstexprValue(std::string_view sv) : length(0), value{}
    {
        for (size_t i = 0; i < sv.size() && i < N; ++i)
        {
            value[i] = sv[i];
            ++length;
        }
        value[length] = '\0'; // 确保字符串以 null 终止
    }

    /**
     * @brief 从 std::string 初始化字符数组
     *
     * @param s std::string
     */
    constexpr explicit ConstexprValue(const std::string& s) : length(0), value{}
    {
        for (size_t i = 0; i < s.size() && i < N; ++i)
        {
            value[i] = s[i];
            ++length;
        }
        value[length] = '\0'; // 确保字符串以 null 终止
    }

    /**
     * @brief 从另一个 ConstexprValue<char, N1> 初始化字符数组
     *
     * @param other 另一个 ConstexprValue<char, N1> 对象
     */
    template <size_t N1>
    explicit constexpr ConstexprValue(CONST ConstexprValue<char, N1>& other) : length(0), value{}
    {
        if (other.size() >= N)
        {
            throw std::runtime_error("String is too long");
        }
        auto str = other.c_str();
        while (*str != '\0')
        {
            value[length] = *str;
            ++length;
            ++str;
        }
        value[length] = '\0';
    }

    /**
     * @brief 赋值操作符，从另一个 ConstexprValue<char, N1> 赋值
     *
     * @param other 另一个 ConstexprValue<char, N1> 对象
     * @return 赋值后的对象
     */
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

    /**
     * @brief 连接操作符，将两个 ConstexprValue<char, N1> 连接起来
     *
     * @param other 另一个 ConstexprValue<char, N1> 对象
     * @return 连接后的对象
     */
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
     * @brief 追加操作符，将两个 ConstexprValue<char, N2> 追加到 ConstexprValue<char, N1>
     *
     * @param other 另一个 ConstexprValue<char, N2> 对象
     * @return 追加后的对象
     */
    template <size_t N1, size_t N2>
    consteval ConstexprValue<char, N1> appendAsSameCap(CONST ConstexprValue<char, N2>& other)
    {
        ConstexprValue<char, N1> result("");
        auto ptr = this->c_str();
        while (*ptr != '\0' && result.length < N1)
        {
            result.value[result.length] = *ptr;
            ++(result.length);
            ++ptr;
        }
        ptr = other.c_str();
        while (*ptr != '\0' && result.length < N1)
        {
            result.value[result.length] = *ptr;
            ++(result.length);
            ++ptr;
        }
        return result;
    }

    /**
     * @brief 返回字符串的长度
     *
     * @return 字符串的长度
     */
    [[nodiscard]] constexpr size_t size() const
    {
        return length;
    }

    /**
     * @brief 返回字符串的 C 风格表示
     *
     * @return 字符串的 C 风格表示
     */
    [[nodiscard]] constexpr const char* c_str() const
    {
        return value.data();
    }

    /**
     * @brief 获取指定位置的字符
     *
     * @param index 字符索引
     * @return 指定位置的字符，如果索引超出范围则返回 '\0'
     */
    constexpr char operator[](size_t index) const
    {
        return (index < length) ? value[index] : '\0';
    }

    /**
     * @brief 转换为 std::string_view
     *
     * @return std::string_view
     */
    [[nodiscard]] constexpr std::string_view to_string_view() const
    {
        return std::string_view(c_str(), length);
    }

    /**
     * @brief 转换为 std::string
     *
     * @return std::string
     */
    [[nodiscard]] constexpr std::string to_string() const
    {
        return std::string(c_str(), length);
    }

    /**
     * @brief 与 std::string_view 比较
     *
     * @param sv std::string_view
     * @return 如果相等返回 true，否则返回 false
     */
    constexpr bool operator==(std::string_view sv) const
    {
        if (length != sv.size())
        {
            return false;
        }
        for (size_t i = 0; i < length; ++i)
        {
            if (value[i] != sv[i])
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief 与 std::string 比较
     *
     * @param s std::string
     * @return 如果相等返回 true，否则返回 false
     */
    constexpr bool operator==(const std::string& s) const
    {
        if (length != s.size())
        {
            return false;
        }
        for (size_t i = 0; i < length; ++i)
        {
            if (value[i] != s[i])
            {
                return false;
            }
        }
        return true;
    }
};

/**
 * @brief 常量字符串结构体，继承自 ConstexprValue
 *
 * @tparam N 字符串的长度
 */
template <size_t N>
using ConstexprString = ConstexprValue<char, N>;

/**
 * @brief 预定义的常量字符串类型，长度为32
 */
using ConstexprString32 = ConstexprString<32>;

/**
 * @brief 预定义的常量字符串类型，长度为64
 */
using ConstexprString64 = ConstexprString<64>;

/**
 * @brief 预定义的常量字符串类型，长度为128
 */
using ConstexprString128 = ConstexprString<128>;

/**
 * @brief 预定义的常量字符串类型，长度为256
 */
using ConstexprString256 = ConstexprString<256>;

/**
 * @brief 预定义的常量字符串类型，长度为512
 */
using ConstexprString512 = ConstexprString<512>;

/**
 * @brief 预定义的常量字符串类型，长度为1024
 */
using ConstexprString1024 = ConstexprString<1024>;

/**
 * @brief 计算C风格字符串的长度
 *
 * @param str C风格字符串
 * @return 字符串的长度
 */
consteval size_t sizeOfString(const char *str) {
  size_t i = 0;
  while (*(str+i) != '\0') {
      ++i;
  }
  return i + 1;
}

/**
 * @brief 定义一个常量字符串
 *
 * @param name 常量字符串的名称
 * @param str 字符串字面量
 */
#define CONSTEXPR_STRING_NAMED(name, str)                                      \
  ConstexprString<sizeOfString(str)> name(str)

/**
 * @brief 定义一个常量字符串
 *
 * @param str 字符串字面量
 */
#define CONSTEXPR_STRING(str) CONSTEXPR_STRING_NAMED(, str)

/**
 * @brief 辅助函数，用于从数组创建 ConstexprValue 对象
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
 * @brief 辅助函数，用于从单个值创建 ConstexprValue 对象
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
