#ifndef FUND_ALG_DEQUE_HPP
#define FUND_ALG_DEQUE_HPP

#include "List.hpp"

namespace my_container {

    template <typename T>
    class Deque : public List<T> {
    public:
        Deque() = default;

        Deque(std::initializer_list<T> init) : List<T>(init) {}

        Deque(const Deque& other);

        Deque(Deque&& other) noexcept;

        Deque& operator=(const Deque& other);

        Deque& operator=(Deque&& other) noexcept;

        void push_front(const T& value) override;

        void pop_front() override;

        T& front() override;

        const T& front() const override;

        T& back() override;

        const T& back() const override;

        using List<T>::operator==;
        using List<T>::operator!=;
        using List<T>::operator<;
    };

    template<typename T>
    const T &Deque<T>::back() const {
        return List<T>::back();
    }

    template<typename T>
    T &Deque<T>::back() {
        return List<T>::back();
    }

    template<typename T>
    T &Deque<T>::front() {
        return List<T>::front();
    }

    template<typename T>
    const T &Deque<T>::front() const {
        return List<T>::front();
    }

    template<typename T>
    void Deque<T>::pop_front() {
        List<T>::pop_front();
    }

    template<typename T>
    void Deque<T>::push_front(const T &value) {
        List<T>::push_front(value);
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

}

#endif // FUND_ALG_DEQUE_HPP