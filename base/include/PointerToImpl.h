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
        PointerImpl(IMPL *impl_pointer = new IMPL())
        {
            m_impl.reset(impl_pointer);
        }

        explicit PointerImpl(const PointerImpl &other)
        {
            m_impl = std::make_unique<IMPL>(*other.m_impl);
        }

        PointerImpl(PointerImpl &&other) noexcept
        {
            m_impl.reset(other.m_impl.release());
        }

        PointerImpl &operator=(const PointerImpl &other)
        {
            m_impl = std::make_unique<IMPL>(*other.m_impl);
            return *this;
        }

        PointerImpl &operator=(PointerImpl &&other) noexcept
        {
            m_impl.reset(other.m_impl.release());
            return *this;
        }

        virtual ~PointerImpl() = default;


};

#endif //POINTERTOIMPL_H