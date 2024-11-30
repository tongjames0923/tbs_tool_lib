#ifndef POINTERTOIMPL_H
#define POINTERTOIMPL_H
#include <memory>
#include <tbs/defs.h>

/**
 * @brief 定义了一个重置函数的类型别名。
 *
 * @tparam T 实现类的类型。
 */
template <typename T>
using __resetor = void(T*&);

/**
 * @brief 默认的重置函数，用于删除指针并将其设置为 nullptr。
 *
 * @tparam T 实现类的类型。
 * @param ptr 需要重置的指针。
 */
template <typename T>
void default_resetor(T*& ptr)
{
    if (ptr == nullptr)
    {
        return;
    }
    delete ptr;
    ptr = nullptr;
}

/**
 * @brief 定义了一个重新创建对象的函数类型别名。
 *
 * @tparam T 实现类的类型。
 */
template <typename T>
using __remake_func = T*();

/**
 * @brief 默认的重新创建函数，抛出运行时错误表示无法重新创建指针。
 *
 * @tparam T 实现类的类型。
 * @throws std::runtime_error 抛出运行时错误，表示无法重新创建指针。
 */
template <typename T>
T* no_remake()
{
    throw std::runtime_error("cannot remake pointer");
}

/**
 * @brief 默认的重新创建函数，创建一个新的实现类对象。
 *
 * @tparam T 实现类的类型。
 * @return 返回新创建的实现类对象的指针。
 */
template <typename T>
T* empty_args_remake()
{
    return new T();
}

/**
 * @brief 模板类 PointerImpl 实现了指向实现的指针，用于封装实际的实现细节。
 *
 * 这种模式通常用于隐藏实际的实现细节，提供一种更清晰的接口。
 *
 * @tparam IMPL 实际实现的类类型。
 * @tparam _resetor 重置函数，默认为 default_resetor。
 * @tparam _remake 重新创建函数，默认为 no_remake。
 */
template <typename IMPL, __resetor<IMPL> _resetor = default_resetor<IMPL>, __remake_func<IMPL> _remake = no_remake<IMPL>>
class PointerImpl
{
private:
    using IMPL_PTR = IMPL*;
    // 封装实现类的智能指针，确保资源的正确管理。
    mutable IMPL_PTR m_impl = nullptr;

    static_assert(_resetor != nullptr, "PointerImpl::PointerImpl: cannot reset pointer");
    static_assert(_remake != nullptr, "PointerImpl::PointerImpl: cannot remake pointer");

    /**
     * @brief 如果 m_impl 为 nullptr，则调用 _remake 函数重新创建对象。
     */
    void ifNullImpl() const
    {
        if (m_impl == nullptr)
        {
            m_impl = _remake();
        }
    }

protected:
    /**
     * @brief 获取实现类的常引用。
     *
     * @return 实现类的常引用。
     */
    const IMPL& getImpl() const
    {
        ifNullImpl();
        return *m_impl;
    }

    /**
     * @brief 获取实现类的引用。
     *
     * @return 实现类的引用。
     */
    IMPL& getImpl()
    {
        ifNullImpl();
        return *m_impl;
    }

    /**
     * @brief 重置实现类的指针。
     *
     * @param p 指向新实现类的指针。可以为 nullptr。
     */
    void reset(IMPL* p)
    {
        if (p == this->m_impl)
        {
            return;
        }
        if (m_impl != nullptr)
        {
            _resetor(m_impl);
        }

        m_impl = p;
    }

public:
    /**
     * @brief 构造函数，初始化实现类的指针。
     *
     * @param impl_pointer 指向实现类的指针，默认为 new IMPL()。
     */
    explicit PointerImpl(IMPL* impl_pointer = _remake())
    {
        reset(impl_pointer);
    }

    /**
     * @brief 模板构造函数，用于创建指向指定实现类的指针。
     *
     * @tparam _Args 构造函数参数类型。
     * @param args 构造函数参数。
     */
    template <typename... _Args>
    explicit PointerImpl(_Args&&... args)
    {
        reset(new IMPL(std::forward<_Args>(args)...));
    }

    /**
     * @brief 从另一个 PointerImpl 对象复制数据。
     *
     * @param other 要复制的 PointerImpl 对象。
     */
    void copyFrom(const PointerImpl& other)
    {
        if (other.m_impl != this->m_impl)
        {
            if constexpr (std::is_copy_constructible_v<IMPL>)
            {
                reset(new IMPL(other.getImpl()));
            }
            else if constexpr (std::is_copy_assignable_v<IMPL>)
            {
                getImpl() = other.getImpl();
            }
            else
            {
                throw std::runtime_error("PointerImpl::PointerImpl: cannot copy from other pointer");
            }
        }
    }

    /**
     * @brief 从另一个 PointerImpl 对象移动数据。
     *
     * @param other 要移动的 PointerImpl 对象。
     */
    void moveFrom(PointerImpl&& other) noexcept
    {
        if (other.m_impl == nullptr)
        {
            return;
        }
        reset(other.m_impl);
        other.m_impl = nullptr;
    }

    /**
     * @brief 删除拷贝赋值操作符。
     */
    DELETE_COPY_ASSIGNMENT(PointerImpl)

    /**
     * @brief 删除拷贝构造函数。
     */
    DELETE_COPY_CONSTRUCTION(PointerImpl)

    /**
     * @brief 移动构造函数。
     *
     * @param other 要移动的 PointerImpl 对象。
     */
    PointerImpl(PointerImpl&& other) noexcept
    {
        moveFrom(std::move(other));
    }

    /**
     * @brief 移动赋值操作符。
     *
     * @param other 要移动的 PointerImpl 对象。
     * @return 返回当前对象的引用。
     */
    PointerImpl& operator=(PointerImpl&& other) noexcept
    {
        moveFrom(std::move(other));
        return *this;
    }

    /**
     * @brief 析构函数，释放资源。
     */
    virtual ~PointerImpl()
    {
        reset(nullptr);
    }
};

#endif // POINTERTOIMPL_H
