//
// Created by abstergo on 24-12-2.
//

#ifndef ITERATABLE_H
#define ITERATABLE_H

/**
 * @brief 可迭代接口类。
 *
 * 该类定义了可迭代对象的基本接口，要求实现begin和end方法。
 *
 * @tparam T 迭代器类型。
 */
template <typename T>
class Iteratable
{
public:
    /**
     * @brief 获取迭代器指向容器的第一个元素。
     *
     * @return 指向第一个元素的迭代器。
     */
    virtual T begin() = 0;

    /**
     * @brief 获取迭代器指向容器的最后一个元素之后的位置。
     *
     * @return 指向最后一个元素之后位置的迭代器。
     */
    virtual T end() = 0;
};

/**
 * @brief 常量可迭代接口类。
 *
 * 该类定义了常量可迭代对象的基本接口，要求实现cbegin和cend方法。
 *
 * @tparam T 迭代器类型。
 */
template <typename T>
class IteratableConst
{
public:
    /**
     * @brief 获取常量迭代器指向容器的第一个元素。
     *
     * @return 指向第一个元素的常量迭代器。
     */
    virtual T cbegin() const = 0;

    /**
     * @brief 获取常量迭代器指向容器的最后一个元素之后的位置。
     *
     * @return 指向最后一个元素之后位置的常量迭代器。
     */
    virtual T cend() const = 0;
};

/**
 * @brief 反向可迭代接口类。
 *
 * 该类定义了反向可迭代对象的基本接口，要求实现rbegin和rend方法。
 *
 * @tparam T 迭代器类型。
 */
template <typename T>
class IteratableReverse
{
public:
    /**
     * @brief 获取反向迭代器指向容器的最后一个元素。
     *
     * @return 指向最后一个元素的反向迭代器。
     */
    virtual T rbegin() = 0;

    /**
     * @brief 获取反向迭代器指向容器的第一个元素之前的位置。
     *
     * @return 指向第一个元素之前位置的反向迭代器。
     */
    virtual T rend() = 0;
};

/**
 * @brief 迭代器基类。
 *
 * 该类定义了迭代器的基本操作，包括解引用和递增操作。
 *
 * @tparam T 迭代器指向的元素类型。
 */
template <typename T>
class Iterator
{
public:
    /**
     * @brief 解引用迭代器，获取当前指向的元素。
     *
     * @return 当前指向的元素的引用。
     */
    virtual T& operator*() = 0;

    /**
     * @brief 递增迭代器，移动到下一个元素。
     *
     * @return 递增后的迭代器引用。
     */
    virtual Iterator& operator++() = 0;
};

/**
 * @brief 基础迭代器类。
 *
 * 该类继承自Iterator，并增加了指针和偏移量的管理。
 *
 * @tparam T 迭代器指向的元素类型。
 */
template <typename T>
class BaseIterator : public Iterator<T>
{
private:
    T* m_p;         ///< 数据指针。
    size_t m_offset;  ///< 当前偏移量。

protected:
    /**
     * @brief 获取数据指针的引用。
     *
     * @return 数据指针的引用。
     */
    T*& getPointer()
    {
        return m_p;
    }

    /**
     * @brief 获取偏移量的引用。
     *
     * @return 偏移量的引用。
     */
    size_t& getOffset()
    {
        return m_offset;
    }

    /**
     * @brief 获取常量偏移量的引用。
     *
     * @return 常量偏移量的引用。
     */
    const size_t& getOffset() const
    {
        return m_offset;
    }

    /**
     * @brief 获取常量数据指针的引用。
     *
     * @return 常量数据指针的引用。
     */
    CONST T*& getPointer() const
    {
        return m_p;
    }

public:
    /**
     * @brief 构造一个BaseIterator对象。
     *
     * @param p 数据指针。
     * @param offset 初始偏移量。
     */
    BaseIterator(T* p, CONST size_t& offset) : m_p(p), m_offset(offset)
    {
    }

    /**
     * @brief 比较两个迭代器是否相等。
     *
     * @param other 另一个迭代器。
     * @return 如果两个迭代器相等则返回true，否则返回false。
     */
    virtual bool operator==(const BaseIterator& other) const
    {
        return m_p == other.m_p && m_offset == other.m_offset;
    }

    /**
     * @brief 比较两个迭代器是否不相等。
     *
     * @param other 另一个迭代器。
     * @return 如果两个迭代器不相等则返回true，否则返回false。
     */
    virtual bool operator!=(const BaseIterator& other) const
    {
        return !(*this == other);
    }

    /**
     * @brief 解引用迭代器，获取当前指向的元素。
     *
     * @return 当前指向的元素的引用。
     */
    T& operator*() override
    {
        return *(m_p + m_offset);
    }

    /**
     * @brief 递增迭代器，移动到下一个元素。
     *
     * @return 递增后的迭代器引用。
     * @throw std::runtime_error 如果没有下一个元素。
     */
    Iterator<T>& operator++() override
    {
        if (!hasNext())
        {
            throw std::runtime_error("No next element");
        }
        next();
        return *this;
    }

    /**
     * @brief 检查是否还有更多元素可以遍历。
     *
     * @return 如果有更多元素则返回true，否则返回false。
     */
    [[nodiscard]] virtual bool hasNext() CONST
    {
        return true;
    }

    /**
     * @brief 移动到下一个元素。
     */
    virtual void next() = 0;
};

/**
 * @brief 带大小的基础迭代器类。
 *
 * 该类继承自BaseIterator，并增加了对容器大小的支持。
 *
 * @tparam T 迭代器指向的元素类型。
 */
template <typename T>
class SizedBaseIterator : public BaseIterator<T>
{
private:
    size_t m_len;  ///< 容器大小。

public:
    /**
     * @brief 构造一个SizedBaseIterator对象。
     *
     * @param p 数据指针。
     * @param offset 初始偏移量。
     * @param len 容器大小。
     */
    SizedBaseIterator(T* p, CONST size_t& offset, size_t len) : BaseIterator<T>(p, offset), m_len(len)
    {
    }

    /**
     * @brief 检查是否还有更多元素可以遍历。
     *
     * @return 如果有更多元素则返回true，否则返回false。
     */
    bool hasNext() CONST override
    {
        return this->getOffset() < size();
    }

    /**
     * @brief 获取容器的大小。
     *
     * @return 容器的大小。
     */
    [[nodiscard]] size_t size() const
    {
        return m_len;
    }

    /**
     * @brief 移动到下一个元素。
     */
    void next() override
    {
        ++(this->getOffset());
    }
};

#endif // ITERATABLE_H
