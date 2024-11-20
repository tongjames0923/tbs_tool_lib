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

        ~TestImpl()
        {
            std::cout << "TestImpl::~TestImpl()" << this << std::endl;
        }

        TestImpl(const TestImpl &) = default;
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

        Test()
        {}
};

void tx(Test t)
{
    t.printX();
}

int main(int argc, char *argv[])
{

    Test t;
    t.set(22);
    Test t1(t);
    tx(t1);
    t.printX();
    return 0;
}