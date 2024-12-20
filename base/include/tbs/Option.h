//
// Created by tongj on 2024/12/20.
//

#ifndef TBS_TOOL_LIB_OPTION_H
#define TBS_TOOL_LIB_OPTION_H

#include <memory>
#include "defs.h"

/**
 * @class OptionFactory
 *
 * @brief Option 对象的工厂类。
 *
 * 提供静态方法用于创建不同类型的 Option 对象。
 */
class OptionFactory;

/**
 * @class Option
 *
 * @brief 模板类，表示一个可选值（Optional Value）。
 *
 * 该类可以包含一个值或为空（null）。它提供了检查是否包含值的方法以及访问内部值的方法。
 *
 * @tparam T 类型参数，表示 Option 可以持有的值的类型。
 */
template <typename T>
class Option
{
public:
    /**
     * @brief 检查 Option 是否为空。
     *
     * @return 如果 Option 为空，则返回 true；否则返回 false。
     */
    [[nodiscard]] bool isNull() const
    {
        return m_val == nullptr;
    }

    /**
     * @brief 解引用 Option 以获取内部值。
     *
     * @return 返回内部值的引用。
     */
    T& operator*()
    {
        return *m_val;
    }

    /**
     * @brief 解引用 Option 以获取内部值（常量版本）。
     *
     * @return 返回内部值的常量引用。
     */
    const T& operator*() const
    {
        return *m_val;
    }

    /**
     * @brief 获取指向内部值的指针。
     *
     * @return 返回指向内部值的指针。
     */
    T* operator->()
    {
        return m_val.get();
    }

    /**
     * @brief 获取指向内部值的指针（常量版本）。
     *
     * @return 返回指向内部值的常量指针。
     */
    const T* operator->() const
    {
        return m_val.get();
    }

    /**
     * @brief 显式转换为 T 类型。
     *
     * @return 返回内部值。
     */
    explicit operator T() const
    {
        return *m_val;
    }

    /**
     * @brief 显式转换为布尔值。
     *
     * @return 如果 Option 不为空，则返回 true；否则返回 false。
     */
    explicit operator bool() const
    {
        return !isNull();
    }

    /**
     * @brief 比较两个 Option 对象是否不相等。
     *
     * @tparam U 另一个 Option 对象中值的类型。
     * @param other 要比较的另一个 Option 对象。
     * @return 如果两个 Option 对象都不为空且内部值不相等，或者一个为空而另一个不为空，则返回 true；否则返回 false。
     */
    template <typename U>
    bool operator!=(const Option<U>& other) const
    {
        if (!this->isNull() && !other.isNull())
        {
            return *m_val != *other.m_val;
        }
        return this->isNull() != other.isNull();
    }

    /**
     * @brief 比较两个 Option 对象是否相等。
     *
     * @tparam U 另一个 Option 对象中值的类型。
     * @param other 要比较的另一个 Option 对象。
     * @return 如果两个 Option 对象都为空，或者都不为空且内部值相等，则返回 true；否则返回 false。
     */
    template <typename U>
    bool operator==(const Option<U>& other) const
    {
        return !(*this != other);
    }

    /**
     * @brief 比较 Option<void> 和其他类型的 Option 是否相等。
     *
     * @param other 要比较的另一个 Option 对象。
     * @return 如果当前 Option 为空，则返回 true；否则返回 false。
     */
    bool operator==(const Option<void>& other) const
    {
        return isNull();
    }

    /**
     * @brief 比较 Option<void> 和其他类型的 Option 是否不相等。
     *
     * @param other 要比较的另一个 Option 对象。
     * @return 如果当前 Option 不为空，则返回 true；否则返回 false。
     */
    bool operator!=(const Option<void>& other) const
    {
        return !isNull();
    }

private:
    /**
     * @brief 默认构造函数，初始化为空。
     */
    Option() : m_val(nullptr)
    {
    }

    /**
     * @brief 构造函数，通过右值引用初始化。
     *
     * @param val 要存储的值。
     */
    explicit Option(T&& val) : m_val(std::make_unique<T>(std::move(val)))
    {
    }

    /**
     * @brief 构造函数，通过左值引用初始化。
     *
     * @param val 要存储的值。
     */
    explicit Option(const T& val) : m_val(std::make_unique<T>(val))
    {
    }

    /**
     * @brief 构造函数，通过 nullptr 初始化为空。
     */
    explicit Option(const std::nullptr_t&) : m_val(nullptr)
    {
    }

    /**
     * @brief OptionFactory 是 Option 的友元类。
     */
    friend class OptionFactory;

    /**
     * @brief 存储实际值的智能指针。
     */
    std::unique_ptr<T> m_val;
};

/**
 * @class Option<void>
 *
 * @brief 特化模板类，表示一个空的 Option。
 */
template <>
class Option<void>
{
public:
    /**
     * @brief 比较 Option<void> 和其他类型的 Option 是否不相等。
     *
     * @tparam U 另一个 Option 对象中值的类型。
     * @param other 要比较的另一个 Option 对象。
     * @return 如果另一个 Option 不为空，则返回 true；否则返回 false。
     */
    template <typename U>
    bool operator!=(const Option<U>& other) const
    {
        return !other.isNull();
    }

    /**
     * @brief 比较 Option<void> 和其他类型的 Option 是否相等。
     *
     * @tparam U 另一个 Option 对象中值的类型。
     * @param other 要比较的另一个 Option 对象。
     * @return 如果另一个 Option 为空，则返回 true；否则返回 false。
     */
    template <typename U>
    bool operator==(const Option<U>& other) const
    {
        return other.isNull();
    }

    /**
     * @brief 检查 Option 是否为空。
     *
     * @return 总是返回 true，因为 Option<void> 始终为空。
     */
    [[nodiscard]] bool isNull() const
    {
        return true;
    }

    /**
     * @brief 提供一个默认值，当 Option 为空时使用。
     *
     * @tparam U 提供的默认值类型。
     * @param val 默认值。
     * @return 返回包含默认值的新 Option 对象。
     */
    template <typename U>
    Option<U> operator|(U&& val) const
    {
        return Option<U>(std::forward<U>(val));
    }
};

/**
 * @typedef NoneOption
 *
 * @brief 定义 Option<void> 的别名。
 */
using NoneOption = Option<void>;

/**
 * @class OptionFactory
 *
 * @brief Option 对象的工厂类。
 */
class OptionFactory
{
public:
    /**
     * @brief 创建一个新的 Option 对象，通过右值引用初始化。
     *
     * @tparam T 要存储的值的类型。
     * @param val 要存储的值。
     * @return 返回新的 Option 对象。
     */
    template <typename T>
    static Option<T> of(T&& val)
    {
        return Option<T>(std::forward<T>(val));
    }

    /**
     * @brief 创建一个新的 Option 对象，通过左值引用初始化。
     *
     * @tparam T 要存储的值的类型。
     * @param val 要存储的值。
     * @return 返回新的 Option 对象。
     */
    template <typename T>
    static Option<T> of(const T& val)
    {
        return Option<T>(val);
    }

    /**
     * @brief 创建一个新的空 Option 对象。
     *
     * @return 返回一个新的空 Option 对象。
     */
    static NoneOption nullOf()
    {
        return {};
    }
};

/**
 * @def SOME_OPTION(val)
 *
 * @brief 宏定义，用于创建一个新的 Option 对象。
 *
 * @param val 要存储的值。
 */
#define SOME_OPTION(val) OptionFactory::of(val)

/**
 * @def NONE_OPTION
 *
 * @brief 宏定义，用于创建一个新的空 Option 对象。
 */
#define NONE_OPTION OptionFactory::nullOf()

#endif // TBS_TOOL_LIB_OPTION_H