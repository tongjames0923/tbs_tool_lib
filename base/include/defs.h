//
// Created by Abstergo on 24-6-22.
//

#ifndef DEFS_H
#define DEFS_H

#include <memory>
#include <mutex>

/**
 * 常量引用
 */
#define CR(type, n) CONST type &n

#define CR_NONE_NAME(type) CONST type &

/**
 * 引用
 */
#define REF(type, n) type &n

/**
 * 虚析构
 */
#define DESTRUCTION(type) virtual ~type() = default;

/**
 * 只读
 */
#define CONST const

/**
 * 常量表达式
 */
#define CONST_EXP constexpr

/**
 * 无异常
 */
#define NO_EXCEPT noexcept

/**
 * 判断跑错
 */
#define ASSERT(exp, text) \
    if (exp)              \
        throw tbs::base_error(text);

/**
 * @brief   1为开启
 *
 */
#define ON 1

/**
 * @brief 0为关闭
 *
 */
#define OFF 0

namespace tbs
{
    using byte = unsigned char;
    using u_size = unsigned long;
    using base_error = std::runtime_error;
    using str_type = std::string;
    using sys_unique_lock = std::unique_lock<std::mutex>;
} // namespace tbs
#endif // DEFS_H
