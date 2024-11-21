//
// Created by tongj on 24-11-20.
//

#ifndef POINTERTOIMPL_H
#define POINTERTOIMPL_H


#include <defs.h>
#include <memory>

template<typename IMPL>
class PointerImpl
{
    private:
        std::unique_ptr<IMPL> m_impl;
    protected:
        CONST IMPL &getImpl() const
        {
            return *m_impl;
        }

        IMPL &getImpl()
        {
            return *m_impl;
        }
    public:
        explicit PointerImpl(IMPL *impl_pointer = new IMPL())
        {
            m_impl.reset(impl_pointer);
        }

        template<typename T=IMPL>

        explicit PointerImpl(std::enable_if_t<std::is_copy_constructible_v<T>, const PointerImpl &> other)
        {
            m_impl = std::make_unique<IMPL>(*other.m_impl);
        }

        template<typename T=IMPL>
        explicit PointerImpl(std::enable_if_t<std::is_move_constructible_v<T>, PointerImpl &&> other) noexcept
        {
            m_impl.reset(other.m_impl.release());
        }

        template<typename T=IMPL>
        PointerImpl &operator=(std::enable_if_t<std::is_copy_assignable_v<T>, const PointerImpl &> other)
        {
            m_impl = std::make_unique<IMPL>(*other.m_impl);
            return *this;
        }

        template<typename T=IMPL>
        PointerImpl &operator=(
                typename std::enable_if_t<std::is_move_assignable_v<T>, PointerImpl &&> other) noexcept
        {
            m_impl.reset(other.m_impl.release());
            return *this;
        }

        virtual ~PointerImpl() = default;
};

#endif //POINTERTOIMPL_H