//
// Created by abstergo on 24-11-11.
//
#include <iostream>
#include <log/loggers/BuiltInLogger.h>
#include <match/match_macro.h>
#include <time_utils.hpp>
#include <PointerToImpl.h>

class TestImpl
{
    public:
        TestImpl()
        {
            std::cout << "TestImpl::TestImpl()" << this << std::endl;
        }

        int x = 0;

        TestImpl(const TestImpl &) = default;

        // TestImpl(TestImpl &&other) noexcept
        // {
        //     std::cout << "TestImpl::TestImpl(TestImpl &&other)" << this << std::endl;
        //     x = std::move(other.x);
        // }
        //
        // TestImpl &operator=(TestImpl &&other) noexcept
        // {
        //     std::cout << "TestImpl::operator=(TestImpl &&other)" << this << std::endl;
        //     x = std::move(other.x);
        //     return *this;
        // }

        ~TestImpl()
        {
            std::cout << "TestImpl::~TestImpl()" << this << std::endl;
        }

};

class Test : public virtual PointerImpl<TestImpl>
{
    public:
        void printX()
        {
            std::cout << "x = " << getImpl().x << std::endl;
        }

        void set(int x)
        {
            getImpl().x = x;
        }

};

void tx(Test t)
{
    t.printX();
}

int main(int argc, char *argv[])
{

    Test t;
    t.set(22);
    Test t1(std::move(t));
    tx(std::move(t1));
    t.printX();
    return 0;
}