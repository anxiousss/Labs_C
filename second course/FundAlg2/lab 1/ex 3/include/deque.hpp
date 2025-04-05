#ifndef FUND_ALG_DEQUE_HPP
#define FUND_ALG_DEQUE_HPP

#include "List.hpp"

namespace my_container {

    template<typename T>
    class Deque : public List<T> {
    public:
        Deque() = default;

        Deque(std::initializer_list<T> init) : List<T>(init) {}

        Deque(const Deque &other);

        Deque(Deque &&other) noexcept;

        Deque &operator=(const Deque &other);

        Deque &operator=(Deque &&other) noexcept;

        T &at(size_t pos);

        T &operator[](size_t pos);

        bool operator==(const Container<T>& other) const override;
        bool operator!=(const Container<T>& other) const override;
        bool operator<(const Container<T>& other) const;
        bool operator>(const Container<T>& other) const;
        bool operator<=(const Container<T>& other) const;
        bool operator>=(const Container<T>& other) const;

        auto operator<=>(const List<T>& other) const {
            return std::lexicographical_compare_three_way(
                    List<T>::cbegin(), List<T>::cend(),
                    other.cbegin(), other.cend()
            );
        }

    };

    template<typename T>
    bool Deque<T>::operator>=(const Container<T> &other) const {
        return !(*this < other);
    }

    template<typename T>
    bool Deque<T>::operator<=(const Container<T> &other) const {
        return !(*this > other);
    }

    template<typename T>
    bool Deque<T>::operator>(const Container<T> &other) const {
        return static_cast<const List<T>&>(other) < static_cast<const List<T>&>(*this);
    }

    template<typename T>
    bool Deque<T>::operator==(const Container<T>& other) const {
        return static_cast<const List<T>&>(*this) == static_cast<const List<T>&>(other);
    }

    template<typename T>
    bool Deque<T>::operator!=(const Container<T>& other) const {
        return !(*this == other);
    }

    template<typename T>
    bool Deque<T>::operator<(const Container<T>& other) const {
        return static_cast<const List<T>&>(*this) < static_cast<const List<T>&>(other);
        }
    template<typename T>
    Deque<T> &Deque<T>::operator=(Deque &&other) noexcept {
        List<T>::operator=(std::move(other));
        return *this;
    }

    template<typename T>
    Deque<T> &Deque<T>::operator=(const Deque &other) {
        List<T>::operator=(other);
        return *this;
    }

    template<typename T>
    Deque<T>::Deque(Deque &&other) noexcept : List<T>(std::move(other)) {}

    template<typename T>
    Deque<T>::Deque(const Deque &other) : List<T>(other) {}



    template <typename T>
    T &Deque<T>::operator[](size_t pos) {
        auto it = this->begin();
        std::advance(it, pos);
        return *it;
    }

    template <typename T>
    T &Deque<T>::at(size_t pos) {
        if (pos >= this->size()) {
            throw std::out_of_range("At");
        }
        return (*this)[pos];
    }

}

#endif // FUND_ALG_DEQUE_HPP