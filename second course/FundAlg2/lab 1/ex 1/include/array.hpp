

#ifndef FUND_ALG_ARRAY_HPP
#define FUND_ALG_ARRAY_HPP

#include <algorithm>
#include <compare>
#include <iostream>
#include <iterator>

#include "container.hpp"

namespace my_container {

    template <typename T, size_t N>
    class Array final : public Container<T> {
    private:
        T elements[N]{};

    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using iterator = T*;
        using const_iterator = const T*;
        using difference_type = ptrdiff_t;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        Array() = default;
        explicit Array(std::initializer_list<T> init) ;
        ~Array() = default;
        Array(const Array<T, N>& other);
        Array(Array<T, N>&& other) noexcept;
        Array<T, N>& operator=(const Array<T, N>& other);
        Array<T, N>& operator=(Array<T, N>&& other) noexcept;
        Array& operator=(std::initializer_list<T> init);


        T& at(size_t index);
        const T& at(size_t index) const;
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        T* data() noexcept;
        const T* data() const noexcept;


        iterator begin() noexcept;
        const_iterator cbegin() const noexcept override;
        iterator end() noexcept;
        const_iterator cend() const noexcept override;

        reverse_iterator rbegin() noexcept;
        const_reverse_iterator crbegin() const noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator crend() const noexcept;

        bool empty() const noexcept override;
        size_t size() const noexcept override;
        size_t max_size() const noexcept override;

        void fill(const T& value);
        void swap(Array& other) noexcept;

        bool operator==(const Array<T, N>& rhs) const;
        std::strong_ordering operator<=>(const Array<T, N>& rhs) const;
    };


    template <typename T, size_t N>
    Array<T, N>::Array(std::initializer_list<T> init) {
        if (static_cast<size_t>(init.size()) != N) {
            throw std::invalid_argument("Invalid initializer list size");
        }
        std::copy(init.begin(), init.end(), elements);
    }

    template <typename T, size_t N>
    Array<T, N>::Array(const Array<T, N>& other) {
        std::copy(other.cbegin(), other.cend(), this->begin());
    }

    template <typename T, size_t N>
    Array<T, N>::Array(Array<T, N>&& other) noexcept {
        std::move(other.begin(), other.end(), this->begin());
        other.fill(T{});
    }

    template <typename T, size_t N>
    Array<T, N>& Array<T, N>::operator=(const Array<T, N>& other) {
        if (this != &other) {
            std::copy(other.cbegin(), other.cend(), this->begin());
        }
        return *this;
    }

    template <typename T, size_t N>
    Array<T, N>& Array<T, N>::operator=(Array<T, N>&& other)  noexcept {
        if (this != &other) {
            std::move(other.begin(), other.end(), begin());
            other.fill(T{});
        }
        return *this;
    }

    template <typename T, size_t N>
    Array<T, N>& Array<T, N>::operator=(std::initializer_list<T> init) {
        if (init.size() != N) {
            throw std::invalid_argument("Invalid initializer list size");
        }
        std::copy(init.begin(), init.end(), begin());
        return *this;
    }

    template <typename T, size_t N>
    T& Array<T, N>::at(size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    template <typename T, size_t N>
    const T& Array<T, N>::at(size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    template <typename T, size_t N>
    T& Array<T, N>::operator[](size_t index) {
        return elements[index];
    }

    template <typename T, size_t N>
    const T& Array<T, N>::operator[](size_t index) const {
        return elements[index];
    }

    template <typename T, size_t N>
    T& Array<T, N>::front() {
        if (empty()) throw std::out_of_range("Array is empty");
        return elements[0];
    }

    template <typename T, size_t N>
    const T& Array<T, N>::front() const {
        if (empty()) throw std::out_of_range("Array is empty");
        return elements[0];
    }

    template <typename T, size_t N>
    T& Array<T, N>::back() {
        static_assert(N > 0, "Array<0> is empty");
        return elements[N-1];
    }

    template <typename T, size_t N>
    const T& Array<T, N>::back() const {
        static_assert(N > 0, "Array<0> is empty");
        return elements[N-1];
    }

    template <typename T, size_t N>
    T* Array<T, N>::data() noexcept {
        return elements;
    }

    template <typename T, size_t N>
    const T* Array<T, N>::data() const noexcept {
        return elements;
    }

    template <typename T, size_t N>
    typename Array<T, N>::iterator Array<T, N>::begin() noexcept {
        return elements;
    }

    template <typename T, size_t N>
    typename Array<T, N>::const_iterator Array<T, N>::cbegin() const noexcept {
        return elements;
    }

    template <typename T, size_t N>
    typename Array<T, N>::iterator Array<T, N>::end() noexcept {
        return elements + N;
    }

    template <typename T, size_t N>
    typename Array<T, N>::const_iterator Array<T, N>::cend() const noexcept {
        return elements + N;
    }

    template <typename T, size_t N>
    typename Array<T, N>::reverse_iterator Array<T, N>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template <typename T, size_t N>
    typename Array<T, N>::const_reverse_iterator Array<T, N>::crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    template <typename T, size_t N>
    typename Array<T, N>::reverse_iterator Array<T, N>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template <typename T, size_t N>
    typename Array<T, N>::const_reverse_iterator Array<T, N>::crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    template <typename T, size_t N>
    bool Array<T, N>::empty() const noexcept {
        return size() == 0;
    }

    template <typename T, size_t N>
    size_t Array<T, N>::size() const noexcept {
        return N;
    }

    template <typename T, size_t N>
    size_t Array<T, N>::max_size() const noexcept {
        return N;
    }

    template <typename T, size_t N>
    void Array<T, N>::fill(const T& value) {
        std::fill(begin(), end(), value);
    }

    template <typename T, size_t N>
    void Array<T, N>::swap(Array& other) noexcept {
        std::swap_ranges(begin(), end(), other.begin());
    }

    template <typename T, size_t N>
    bool Array<T, N>::operator==(const Array<T, N>& rhs) const {
        return (*this <=> rhs) == std::strong_ordering::equal;
    }

    template <typename T, size_t N>
    std::strong_ordering Array<T, N>::operator<=>(const Array<T, N>& rhs) const {
        return std::lexicographical_compare_three_way(
                cbegin(), cend(),
                rhs.cbegin(), rhs.cend()
        );
    }

    template <typename T>
    class Array<T, 0> {
    public:
        using value_type = T;
        using iterator = T*;
        using const_iterator = const T*;

        Array() = default;
        Array(const Array&) = default;
        Array(Array&&) noexcept = default;
        Array& operator=(const Array&) = default;
        Array& operator=(Array&&) noexcept = default;
        ~Array() = default;

        T& at(size_t) {
            throw std::out_of_range("Array<0> has no elements");
        }
        const T& at(size_t) const {
            throw std::out_of_range("Array<0> has no elements");
        }
        T& operator[](size_t) {
            throw std::out_of_range("Array<0> has no elements");
        }
        const T& operator[](size_t) const {
            throw std::out_of_range("Array<0> has no elements");
        }
        T& front() {
            throw std::out_of_range("Array<0> has no elements");
        }
        const T& front() const {
            throw std::out_of_range("Array<0> has no elements");
        }
        T& back() {
            throw std::out_of_range("Array<0> has no elements");
        }
        const T& back() const {
            throw std::out_of_range("Array<0> has no elements");
        }

        iterator begin() noexcept { return nullptr; }
        const_iterator cbegin() const noexcept { return nullptr; }
        iterator end() noexcept { return nullptr; }
        const_iterator cend() const noexcept { return nullptr; }

        size_t size() const noexcept { return 0; }
        size_t max_size() const noexcept { return 0; }
        bool empty() const noexcept { return true; }

        void fill(const T&) noexcept {}
        void swap(Array&) noexcept {}

        bool operator==(const Array&) const noexcept { return true; }
        std::strong_ordering operator<=>(const Array&) const noexcept {
            return std::strong_ordering::equal;
        }
    };

}


#endif //FUND_ALG_ARRAY_HPP
