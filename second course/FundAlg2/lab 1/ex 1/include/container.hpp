
#ifndef FUND_ALG_CONTAINER_HPP
#define FUND_ALG_CONTAINER_HPP

#include <iostream>

template <typename T>
class Container {
public:
    using iterator = T*;
    using const_iterator = const T*;
    Container() = default;
    Container(const Container& container) = default;
    Container& operator=(const Container& container) = default;
    virtual ~Container() = default;

    bool operator==(const Container& other) const {
        if (size() != other.size()) return false;
        return std::equal(cbegin(), cend(), other.cbegin());
    }

    bool operator!=(const Container& other) const {
        return !(*this == other);
    }

    virtual size_t size() const = 0;
    virtual size_t max_size() const = 0;
    virtual bool empty() const = 0;

protected:
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
};

#endif //FUND_ALG_CONTAINER_HPP
