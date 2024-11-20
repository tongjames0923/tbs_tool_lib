
//
// Created by abstergo on 24-11-20.
//

#ifndef CONCURRENTUNORDEREDMAP_H
#define CONCURRENTUNORDEREDMAP_H

#include <concurrency/containers/ConcurrentContainer.h>
#include <unordered_map>

namespace tbs::concurrency::containers
{
/**
 * @brief 线程安全的无序映射。
 *
 * 该类模板通过继承自 `ConcurrentContainer` 提供了一个线程安全的无序映射实现。
 * 它使用细粒度的锁机制来确保读写操作的线程安全性。
 *
 * @tparam K 映射的键类型。
 * @tparam V 映射的值类型。
 * @tparam LOCK 锁适配器类型，默认为 `SharedMutexLockAdapter`。
 */
template<typename K, typename V, typename LOCK = SharedMutexLockAdapter>
class ConcurrentUnorderedMap
    : public virtual tbs::concurrency::containers::ConcurrentContainer<std::unordered_map<K, V>, LOCK>
{
    private:
        using Base = tbs::concurrency::containers::ConcurrentContainer<std::unordered_map<K, V>, LOCK>;
    public:
        /**
         * @brief 清空映射中的所有元素。
         */
        void clear()
        {
            Base::writeAsAtomic([](std::unordered_map<K, V> &map) { map.clear(); });
        }

        /**
         * @brief 删除指定键的元素。
         *
         * @param key 要删除的键。
         * @return 返回实际删除的元素数量。
         */
        size_t erase(const K &key)
        {
            size_t r = 0;
            Base::writeAsAtomic([&key, &r](std::unordered_map<K, V> &map) { r = map.erase(key); });
            return r;
        }

        /**
         * @brief 检查映射是否为空。
         *
         * @return 如果映射为空则返回 `true`，否则返回 `false`。
         */
        bool empty() const
        {
            bool r = false;
            Base::readAsAtomic([&r](const std::unordered_map<K, V> &map) { r = map.empty(); });
            return r;
        }

        /**
         * @brief 获取映射中的元素数量。
         *
         * @return 返回映射中的元素数量。
         */
        size_t size() const
        {
            size_t r = 0;
            Base::readAsAtomic([&r](const std::unordered_map<K, V> &map) { r = map.size(); });
            return r;
        }

        /**
         * @brief 插入一个键值对。
         *
         * @param value 要插入的键值对。
         */
        void insert(const std::pair<K, V> &value)
        {
            insert({value.first, value.second});
        }

        /**
         * @brief 插入一个键值对（右值引用）。
         *
         * @param value 要插入的键值对。
         */
        void insert(std::pair<K, V> &&value)
        {
            insert({std::move(value.first), std::move(value.second)});
        }

        /**
         * @brief 插入多个键值对。
         *
         * @param values 要插入的键值对列表。
         */
        void insert(std::initializer_list<std::pair<K, V>> &&values)
        {
            Base::writeAsAtomic([&values](std::unordered_map<K, V> &map) { map.insert(values); });
        }

        /**
         * @brief 检查映射中是否包含指定的键。
         *
         * @param key 要检查的键。
         * @return 如果映射中包含该键则返回 `true`，否则返回 `false`。
         */
        bool contains(const K &key) const
        {
            bool r = false;
            Base::readAsAtomic([&key, &r](const std::unordered_map<K, V> &map) { r = map.contains(key); });
            return r;
        }

        /**
         * @brief 获取指定键的值。
         *
         * @param key 要获取的键。
         * @return 返回指定键的值。
         */
        V operator[](const K &key) CONST
        {
            return at(key);
        }

        /**
         * @brief 获取指定键的值。
         *
         * @param key 要获取的键。
         * @return 返回指定键的值。
         */
        V at(const K &key) CONST
        {
            V r;
            Base::readAsAtomic([&key, &r](const std::unordered_map<K, V> &map) { r = map.at(key); });
            return r;
        }

        /**
         * @brief 遍历映射中的所有元素，并对每个元素执行给定的函数。
         *
         * @param f 要执行的函数，接受一个常量引用参数。
         */
        void foreach (std::function<bool(CONST std::pair<K, V> &)> f) CONST
        {
            if (f == nullptr)
            {
                return;
            }
            Base::readAsAtomic(
                    [f](const std::unordered_map<K, V> &map)
                    {
                        for (auto &p : map)
                        {
                            if (!f(p))
                            {
                                break;
                            }
                        }
                    });
        }

        /**
         * @brief 遍历映射中的所有元素，并对每个元素执行给定的函数。
         *
         * @param f 要执行的函数，接受一个非常量引用参数。
         */
        void foreach (std::function<bool(std::pair<CONST K, V> &)> f)
        {
            if (f == nullptr)
            {
                return;
            }
            Base::writeAsAtomic(
                    [f](std::unordered_map<K, V> &map)
                    {
                        for (auto &p : map)
                        {
                            if (!f(p))
                            {
                                break;
                            }
                        }
                    });
        }

        /**
         * @brief 如果映射中存在指定的键，则对其值执行给定的操作。
         *
         * @param key 要检查的键。
         * @param operation 要执行的操作，接受一个非常量引用参数。
         */
        void operateIfExists(const K &key, std::function<void(V &)> &&operation)
        {
            operateIfExists(key, [&operation](V &value, std::unordered_map<K, V> &map) { operation(value); }, nullptr);
        }

        /**
         * @brief 如果映射中存在指定的键，则对其值执行给定的操作。
         *
         * @param key 要检查的键。
         * @param operation 要执行的操作，接受一个常量引用参数。
         */
        void operateIfExists(const K &key, std::function<void(CONST V &)> &&operation) const
        {
            if (operation == nullptr)
            {
                return;
            }
            Base::readAsAtomic(
                    [&key, &operation](const std::unordered_map<K, V> &map)
                    {
                        if (map.contains(key))
                        {
                            operation(map.at(key));
                        }
                    });
        }

        /**
         * @brief 如果映射中存在指定的键，则对其值执行给定的操作；如果不存在，则执行另一个操作。
     *
     * @param key 要检查的键。
     * @param operation 要执行的操作，接受一个非常量引用参数和映射本身。
     * @param notFound 如果键不存在时要执行的操作，接受映射本身。
     */
    void operateIfExists(
            const K &key, std::function<void(V &, std::unordered_map<K, V> &map)> &&operation,
            std::function<void(std::unordered_map<K, V> &map)> &&notFound)
    {
        if (operation == nullptr)
        {
            return;
        }

        Base::writeAsAtomic(
                [&key, &operation, &notFound](std::unordered_map<K, V> &map)
                {
                    if (map.contains(key))
                    {
                        operation(map.at(key), map);
                    }
                    else
                    {
                        if (notFound != nullptr)
                        {
                            notFound(map);
                        }
                    }
                });
    }
};
} // namespace tbs::concurrency::containers

#endif // CONCURRENTUNORDEREDMAP_H
