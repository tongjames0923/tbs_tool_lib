//
// Created by abstergo on 2024/12/21.
//

#ifndef TBS_TOOL_LIB_BASE_INCLUDE_TBS_OPTION_NULLOPTION_H
#define TBS_TOOL_LIB_BASE_INCLUDE_TBS_OPTION_NULLOPTION_H

#include <utility>
template <typename T>
class Option;
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
     * @brief 将 Option<void> 转换为 bool 类型。
     *
     * @return 总是返回 false，因为 Option<void> 始终为空。
     */
    operator bool() const
    {
        return false;
    }

    /**
     * @brief 获取 Option<void> 的值。
     *
     * @return 总是返回 nullptr，因为 Option<void> 始终为空。
     */
    std::nullptr_t operator*() const
    {
        return nullptr;
    }
    /**
     * @brief 提供一个默认值，当 Option 为空时使用。
     *
     * @tparam U 提供的默认值类型。
     * @param val 默认值。
     * @return 返回包含默认值的新 Option 对象。
     */
    template <typename U>
    Option<U> operator<<(U&& val) const
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


#endif // TBS_TOOL_LIB_BASE_INCLUDE_TBS_OPTION_NULLOPTION_H
