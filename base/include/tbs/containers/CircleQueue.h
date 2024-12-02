// Created by abstergo on 24-12-2.
//

#ifndef CIRCLEQUEUE_H
#define CIRCLEQUEUE_H
#include "../defs.h"
#include "iterator/Iteratable.h"

template <typename T> class CircleQueueIterator : public Iterator<T> {
private:
  T *m_data;
  size_t m_index;
  size_t m_size;
  size_t m_capacity;

public:
  CircleQueueIterator(T *data, CONST size_t &index, CONST size_t &capacity)
      : m_data(data), m_index(index), m_capacity(capacity) {}
  T &operator*() override { return m_data[m_index]; }
  T *operator->() override { return &m_data[m_index]; }
  Iterator<T> &operator++() override {
    m_index = (m_index + 1) % m_capacity;
    return *this;
  }
  Iterator<T> &operator--() override {
    m_index = (m_index - 1 + m_capacity) % m_capacity;
    return *this;
  }
  bool operator!=(CONST CircleQueueIterator<T> &other) {
    return this->m_data != other.m_data || m_index != other.m_index;
  }
};

template <typename T, size_t N>
class CircleQueue : public virtual Iteratable<CircleQueueIterator<T>> {
private:
  constexpr static size_t CAPACITY = N + 1;
  std::array<T, CAPACITY> m_data;
  size_t m_tail = 0, m_head = 0;
  friend class CircleQueueIterator<T>;

public:
  CircleQueue(const CircleQueue &) = default;
  CircleQueue &operator=(const CircleQueue &) = default;
  CircleQueue(CircleQueue &&) = default;
  CircleQueue &operator=(CircleQueue &&) = default;
  CircleQueueIterator<T> begin() override {
    return CircleQueueIterator<T>(m_data.data(), m_head, CAPACITY);
  }
  CircleQueueIterator<T> end() override {
    return CircleQueueIterator<T>(m_data.data(), m_tail, CAPACITY);
  }
  [[nodiscard]] size_t size() const {
    auto r = m_tail <= m_head ? CAPACITY - m_head + m_tail : m_tail - m_head;
    return r;
  }

  CircleQueue(CONST std::initializer_list<T> &list) {
    if (list.size() >= N) {
      throw std::runtime_error("size too big");
    }
    for (auto &elem : list) {
      push(elem);
    }
  }
  explicit CircleQueue(CONST T *ptr = nullptr, size_t size = 0) {
    if (ptr == nullptr || size == 0) {
      return;
    }
    if (size >= N) {
      throw std::runtime_error("size too big");
    }
    for (size_t i = 0; i < size; ++i) {
      push(ptr[i]);
    }
  }
  [[nodiscard]] bool empty() const { return m_head == m_tail; }
  [[nodiscard]] bool full() const { return m_head == (m_tail + 1) % CAPACITY; }
  void push(const T &elem) {
    if (full()) {
      return;
    }
    m_data[m_tail] = elem;
    m_tail = (m_tail + 1) % CAPACITY;
  }
  void pop() {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    m_head = (m_head + 1) % CAPACITY;
  }

  T pollFront() {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    auto old = m_head;
    m_head = (m_head + 1) % CAPACITY;
    return m_data[old];
  }
  T pollBack() {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    auto old = m_tail;
    m_tail = (m_tail - 1 + CAPACITY) % CAPACITY;
    return m_data[old];
  }
  CONST T &front() const {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return m_data[m_head];
  }
  T &front() {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return m_data[m_head];
  }
  CONST T &back() const {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return m_data[m_tail];
  }
  T &back() {
    if (empty()) {
      throw std::runtime_error("empty queue");
    }
    return m_data[m_tail];
  }
};

#endif // CIRCLEQUEUE_H
