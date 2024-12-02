//
// Created by abstergo on 24-12-2.
//

#ifndef ITERATABLE_H
#define ITERATABLE_H

template <typename T> class Iteratable {
public:
    virtual T begin() = 0;
    virtual T end() = 0;
};
template <typename T> class IteratableConst {
public:
    virtual T cbegin() const = 0;
    virtual T cend() const = 0;
};
template <typename T> class IteratableReverse {
public:
    virtual T rbegin() = 0;
    virtual T rend() = 0;
};

template <typename T> class Iterator {
public:
    virtual T &operator*() = 0;
    virtual T *operator->() = 0;
    virtual Iterator &operator++() = 0;
    virtual Iterator &operator--() = 0;
};

#endif // ITERATABLE_H
