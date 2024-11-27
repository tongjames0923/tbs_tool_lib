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

// 定义复制构造函数的宏
#define COPY_CONSTRUCTION(n,c) n(CONST n& o){c};

// 定义移动构造函数的宏
#define MOVE_CONSTRUCTION(n,c) n(n&& o) noexcept{c};

// 定义复制赋值运算符的宏
#define COPY_ASSIGNMENT(n,c) n& operator=(CONST n& o){c;return *this;}

// 定义移动赋值运算符的宏
#define MOVE_ASSIGNMENT(n,c) n& operator=(n&& o) noexcept{c;return *this;}

// 定义默认构造函数的宏
#define DEFAULT_CONSTRUCTION(n) n() = default;

// 定义默认析构函数的宏
#define DEFAULT_DESTRUCTION(n) virtual ~n() = default;

// 定义默认复制构造函数的宏
#define DEFAULT_COPY_CONSTRUCTION(n) n(CONST n&) = default;

// 定义默认移动构造函数的宏
#define DEFAULT_MOVE_CONSTRUCTION(n) n(n&&) noexcept = default;

// 定义默认复制赋值运算符的宏
#define DEFAULT_COPY_ASSIGNMENT(n) n& operator=(CONST n&) = default;

// 定义默认移动赋值运算符的宏
#define DEFAULT_MOVE_ASSIGNMENT(n) n& operator=(n&&) noexcept = default;

// 定义删除复制构造函数的宏
#define DELETE_COPY_CONSTRUCTION(n) n(CONST n&) = delete;

// 定义删除移动构造函数的宏
#define DELETE_MOVE_CONSTRUCTION(n) n(n&&) noexcept = delete;

// 定义删除复制赋值运算符的宏
#define DELETE_COPY_ASSIGNMENT(n) n& operator=(CONST n&) = delete;

// 定义删除移动赋值运算符的宏
#define DELETE_MOVE_ASSIGNMENT(n) n& operator=(n&&) noexcept = delete;


namespace tbs
{
using byte            = unsigned char;
using u_size          = unsigned long;
using base_error      = std::runtime_error;
using str_type        = std::string;
using sys_unique_lock = std::unique_lock<std::mutex>;
} // namespace tbs
#endif // DEFS_H