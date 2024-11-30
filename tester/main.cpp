#include <array>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>

#include <tbs/compile_time_function.h>
#include <tbs/constexpr_value.h>
#include <tbs/time_utils.hpp>

#include "../base/include/tbs/constexpr_value.h"

template <typename T, size_t N>
class FixedQueue
{
private:
    std::array<T, N> m_data;
    size_t m_tail = 0, m_head = 0;

public:
    FixedQueue() = default;
    FixedQueue(const FixedQueue&) = default;
    FixedQueue& operator=(const FixedQueue&) = default;
    FixedQueue(FixedQueue&&) = default;
    FixedQueue& operator=(FixedQueue&&) = default;
    FixedQueue(std::initializer_list<T> list)
    {
        for (auto& elem : list)
        {
            push(elem);
        }
    }
    bool empty() const
    {
        return m_head == m_tail;
    }
    bool full() const
    {
        return m_head == (m_tail + 1) % N;
    }
    void push(const T& elem)
    {
        if (full())
        {
            return;
        }
        m_data[m_tail] = elem;
        m_tail = (m_tail + 1) % N;
    }
    void pop()
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        m_head = (m_head + 1) % N;
    }
    CONST T& front() const
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        return m_data[m_head];
    }
    T& front()
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        return m_data[m_head];
    }
};

using DefaultString = ConstexprString64;
int main()
{
    auto now = time_utils::utils_now();
    bool b1 = true;
    for (int i = 0; i < 1000 * 10000; i++)
    {
        constexpr auto a = DefaultString("hello world");
        constexpr auto b = DefaultString(" are you ok?");
        constexpr auto c = a + b;
        // if (b1)
        // {
        //     std::cout << c.c_str() << std::endl;
        //     b1 = false;
        // }
    }
    std::cout << time_utils::utils_now() - now << std::endl;

    return 0;
}
