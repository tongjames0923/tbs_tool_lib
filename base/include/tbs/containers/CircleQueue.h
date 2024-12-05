// Created by abstergo on 24-12-2.
//

#ifndef CIRCLEQUEUE_H
#define CIRCLEQUEUE_H
#include "../defs.h"
#include "iterator/Iteratable.h"

/**
 * @brief 循环队列迭代器类。
 *
 * 该迭代器允许遍历循环队列中的元素。
 * 它实现了SizedBaseIterator接口。
 *
 * @tparam T 存储在循环队列中的元素类型。
 */
template <typename T>
class CircleQueueIterator : public virtual SizedBaseIterator<T>
{
public:
    /**
     * @brief 构造一个CircleQueueIterator对象。
     *
     * @param data 数据数组的指针。
     * @param index 当前索引位置。
     * @param size 数据数组的大小。
     */
    CircleQueueIterator(T* data, CONST size_t& index, CONST size_t& size) : SizedBaseIterator<T>(data, index, size)
    {
    }

    /**
     * @brief 移动到下一个元素。
     */
    void next() override
    {
        this->getOffset() = (this->getOffset() + 1) % SizedBaseIterator<T>::size();
    }
};

/**
 * @brief 固定大小的循环队列实现。
 *
 * 该类提供了一个固定大小的循环队列，可以存储类型为T的元素。
 * 支持入队、出队、获取队首和队尾元素等功能。
 *
 * @tparam T 存储在循环队列中的元素类型。
 * @tparam N 循环队列的最大容量。
 */
template <typename T, size_t N>
class CircleQueue : public virtual Iteratable<CircleQueueIterator<T>>
{
private:
    constexpr static size_t CAPACITY = N + 1; ///< 实际容量为N+1以区分满队列和空队列。
    std::array<T, CAPACITY> m_data; ///< 数据存储数组。
    size_t m_tail = 0; ///< 队尾索引。
    size_t m_head = 0; ///< 队首索引。
    friend class CircleQueueIterator<T>; ///< 允许迭代器访问私有成员。

public:
    /**
     * @brief 默认拷贝构造函数。
     */
    CircleQueue(const CircleQueue&) = default;

    /**
     * @brief 默认拷贝赋值运算符。
     *
     * @return 引用到当前对象。
     */
    CircleQueue& operator=(const CircleQueue&) = default;

    /**
     * @brief 默认移动构造函数。
     */
    CircleQueue(CircleQueue&&) = default;

    /**
     * @brief 默认移动赋值运算符。
     *
     * @return 引用到当前对象。
     */
    CircleQueue& operator=(CircleQueue&&) = default;

    /**
     * @brief 获取迭代器指向队首元素。
     *
     * @return 指向队首元素的迭代器。
     */
    CircleQueueIterator<T> begin() override
    {
        return CircleQueueIterator<T>(m_data.data(), m_head, CAPACITY);
    }

    /**
     * @brief 获取迭代器指向队尾元素之后的位置。
     *
     * @return 指向队尾元素之后位置的迭代器。
     */
    CircleQueueIterator<T> end() override
    {
        return CircleQueueIterator<T>(m_data.data(), m_tail, CAPACITY);
    }

    /**
     * @brief 获取队列中元素的数量。
     *
     * @return 队列中元素的数量。
     */
    [[nodiscard]] size_t size() const
    {
        return  m_tail <= m_head ? CAPACITY - m_head + m_tail : m_tail - m_head;
    }

    /**
     * @brief 使用初始化列表构造循环队列。
     *
     * @param list 初始化列表。
     * @throw std::runtime_error 如果列表大小大于或等于N。
     */
    CircleQueue(CONST std::initializer_list<T>& list)
    {
        if (list.size() >= N)
        {
            throw std::runtime_error("size too big");
        }
        for (auto& elem : list)
        {
            push(elem);
        }
    }

    /**
     * @brief 使用数组构造循环队列。
     *
     * @param ptr 数组指针。
     * @param size 数组大小。
     * @throw std::runtime_error 如果数组大小大于或等于N。
     */
    explicit CircleQueue(CONST T* ptr = nullptr, size_t size = 0)
    {
        if (ptr == nullptr || size == 0)
        {
            return;
        }
        if (size >= N)
        {
            throw std::runtime_error("size too big");
        }
        for (size_t i = 0; i < size; ++i)
        {
            push(ptr[i]);
        }
    }

    /**
     * @brief 检查队列是否为空。
     *
     * @return 如果队列为空则返回true，否则返回false。
     */
    [[nodiscard]] bool empty() const
    {
        return m_head == m_tail;
    }

    /**
     * @brief 检查队列是否已满。
     *
     * @return 如果队列已满则返回true，否则返回false。
     */
    [[nodiscard]] bool full() const
    {
        return m_head == (m_tail + 1) % CAPACITY;
    }

    /**
     * @brief 向队列末尾添加一个元素。
     *
     * @param elem 要添加的元素。
     */
    void push(const T& elem)
    {
        if (full())
        {
            return;
        }
        m_data[m_tail] = elem;
        m_tail = (m_tail + 1) % CAPACITY;
    }

    /**
     * @brief 移除队首元素。
     *
     * @throw std::runtime_error 如果队列为空。
     */
    void pop()
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        m_head = (m_head + 1) % CAPACITY;
    }

    /**
     * @brief 移除并返回队首元素。
     *
     * @return 队首元素。
     * @throw std::runtime_error 如果队列为空。
     */
    T pollFront()
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        auto old = m_head;
        m_head = (m_head + 1) % CAPACITY;
        return m_data[old];
    }

    /**
     * @brief 移除并返回队尾元素。
     *
     * @return 队尾元素。
     * @throw std::runtime_error 如果队列为空。
     */
    T pollBack()
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        auto old = m_tail;
        m_tail = (m_tail - 1 + CAPACITY) % CAPACITY;
        return m_data[old];
    }

    /**
     * @brief 获取队首元素的常量引用。
     *
     * @return 队首元素的常量引用。
     * @throw std::runtime_error 如果队列为空。
     */
    CONST T& front() const
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        return m_data[m_head];
    }

    /**
     * @brief 获取队首元素的引用。
     *
     * @return 队首元素的引用。
     * @throw std::runtime_error 如果队列为空。
     */
    T& front()
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        return m_data[m_head];
    }

    /**
     * @brief 获取队尾元素的常量引用。
     *
     * @return 队尾元素的常量引用。
     * @throw std::runtime_error 如果队列为空。
     */
    CONST T& back() const
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        return m_data[m_tail];
    }

    /**
     * @brief 获取队尾元素的引用。
     *
     * @return 队尾元素的引用。
     * @throw std::runtime_error 如果队列为空。
     */
    T& back()
    {
        if (empty())
        {
            throw std::runtime_error("empty queue");
        }
        return m_data[m_tail];
    }
};

#endif // CIRCLEQUEUE_H
