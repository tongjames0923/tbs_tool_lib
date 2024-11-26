//
// Created by tongj on 24-11-20.
//

#ifndef POINTERTOIMPL_H
#define POINTERTOIMPL_H

#pragma GCC diagnostic ignored "-Wdelete-incomplete"
#pragma warning(push)
#pragma warning(disable : 4150)


#include <tbs/defs.h>
#include <memory>

class Resetor;

/**
 * 模板类PointerImpl实现了指向实现的指针，用于封装实际的实现细节。
 * 这种模式通常用于隐藏实际的实现细节，提供一种更清晰的接口。
 *
 * @tparam IMPL 实际实现的类类型。
 */
template <typename IMPL, typename R = Resetor>
class PointerImpl
{
private:
    using IMPL_PTR = IMPL*;
    // 封装实现类的智能指针，确保资源的正确管理。
    IMPL_PTR m_impl = nullptr;
    R m_reset;

protected:
    /**
     * 获取实现类的常引用。
     *
     * @return 实现类的常引用。
     */
    CONST IMPL& getImpl() const
    {
        ASSERT(m_impl == nullptr, "PointerImpl::getImpl: nullptr pointer")
        return *m_impl;
    }

    /**
     * 获取实现类的引用。
     *
     * @return 实现类的引用。
     */
    IMPL& getImpl()
    {
        ASSERT(m_impl == nullptr, "PointerImpl::getImpl: nullptr pointer")
        return *m_impl;
    }

    void reset(IMPL* p)
    {

        if (p == this->m_impl)
        {
            return;
        }
        if (m_impl != nullptr)
        {
            m_reset.operator()(m_impl);
        }

        m_impl = p;
    }

public:
    /**
     * 构造函数，初始化实现类的指针。
     *
     * @param impl_pointer 指向实现类的指针，默认为new IMPL()。
     */
    explicit PointerImpl(IMPL* impl_pointer = new IMPL())
    {
        reset(impl_pointer);
    }

    /**
     * 拷贝构造函数，仅当实现类可拷贝构造时有效。
     *
     * @param other 另一个PointerImpl对象。
     */
    template <typename T = IMPL>
    explicit PointerImpl(std::enable_if_t<std::is_copy_constructible_v<T>, const PointerImpl&> other)
    {
        ASSERT(other.m_impl == nullptr, "PointerImpl::PointerImpl: nullptr pointer");
        if (other.m_impl != this->m_impl)
        {
            reset(new T(*other.m_impl));
        }
    }

    explicit PointerImpl(PointerImpl&& other) noexcept
    {
        ASSERT(other.m_impl == nullptr, "PointerImpl::PointerImpl: nullptr pointer");
        if (other.m_impl != this->m_impl)
        {
            reset(other.m_impl);
        }
    }

    /**
     * 拷贝赋值运算符，仅当实现类可拷贝赋值时有效。
     *
     * @param other 另一个PointerImpl对象。
     * @return 返回当前PointerImpl对象的引用。
     */
    template <typename T = IMPL>
    PointerImpl& operator=(std::enable_if_t<std::is_copy_assignable_v<T>, const PointerImpl&> other)
    {
        ASSERT(other.m_impl == nullptr, "PointerImpl::PointerImpl: nullptr pointer");
        if (other.m_impl != this->m_impl)
        {
            T* ptr = new T();
            *ptr = *other.m_impl;
            reset(ptr);
        }

        return *this;
    }

    /**
     * 移动赋值运算符，仅当实现类可移动赋值时有效。
     *
     * @param other 另一个要移动的PointerImpl对象。
     * @return 返回当前PointerImpl对象的引用。
     */
    template <typename T = IMPL>
    PointerImpl& operator=(PointerImpl&& other) noexcept
    {
        ASSERT(other.m_impl == nullptr, "PointerImpl::PointerImpl: nullptr pointer");
        if (other.m_impl != this->m_impl)
        {
            reset(other.m_impl);
        }

        return *this;
    }

    virtual ~PointerImpl()
    {
        reset(nullptr);
        }
};

class Resetor
{
public:
    template<typename T>
    void operator()(T *&ptr)
    {
        if (ptr==nullptr)
        {
            return;
        }
        delete ptr;
        ptr = nullptr;
    }
};

#endif //POINTERTOIMPL_H