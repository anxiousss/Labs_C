
#ifndef FUND_ALG_DEQUE_HPP
#define FUND_ALG_DEQUE_HPP

#include <stdexcept>
#include <initializer_list>
#include <limits>
#include <algorithm>

namespace my_container {

    template <typename T>
    class Deque {
    private:
        struct Node {
            T data;
            Node* prev;
            Node* next;
            Node(const T& data = T(), Node* prev = nullptr, Node* next = nullptr)
                    : data(data), prev(prev), next(next) {}
        };

        Node* front_sentinel;
        Node* back_sentinel;
        size_t deque_size;

        void clear() {
            while (!empty()) pop_back();
        }

    public:
        Deque() : deque_size(0) {
            front_sentinel = new Node();
            back_sentinel = new Node();
            front_sentinel->next = back_sentinel;
            back_sentinel->prev = front_sentinel;
        }

        Deque(std::initializer_list<T> init) : Deque() {
            for (const auto& val : init) push_back(val);
        }

        Deque(const Deque& other) : Deque() {
            for (auto it = other.begin(); it != other.end(); ++it) push_back(*it);
        }
        Deque(Deque&& other) noexcept
                : front_sentinel(other.front_sentinel),
                  back_sentinel(other.back_sentinel),
                  deque_size(other.deque_size) {
            other.front_sentinel = new Node();
            other.back_sentinel = new Node();
            other.front_sentinel->next = other.back_sentinel;
            other.back_sentinel->prev = other.front_sentinel;
            other.deque_size = 0;
        }

        ~Deque() {
            clear();
            delete front_sentinel;
            delete back_sentinel;
        }

        Deque& operator=(const Deque& other) {
            if (this != &other) {
                clear();
                for (auto it = other.begin(); it != other.end(); ++it) push_back(*it);
            }
            return *this;
        }
        Deque& operator=(Deque&& other) noexcept {
            if (this != &other) {
                clear();
                delete front_sentinel;
                delete back_sentinel;

                front_sentinel = other.front_sentinel;
                back_sentinel = other.back_sentinel;
                deque_size = other.deque_size;

                other.front_sentinel = new Node();
                other.back_sentinel = new Node();
                other.front_sentinel->next = other.back_sentinel;
                other.back_sentinel->prev = other.front_sentinel;
                other.deque_size = 0;
            }
            return *this;
        }

        void push_back(const T& value) {
            Node* new_node = new Node(value, back_sentinel->prev, back_sentinel);
            back_sentinel->prev->next = new_node;
            back_sentinel->prev = new_node;
            ++deque_size;
        }

        void push_front(const T& value) {
            Node* new_node = new Node(value, front_sentinel, front_sentinel->next);
            front_sentinel->next->prev = new_node;
            front_sentinel->next = new_node;
            ++deque_size;
        }

        void pop_back() {
            if (empty()) throw std::out_of_range("Deque is empty");
            Node* to_delete = back_sentinel->prev;
            to_delete->prev->next = back_sentinel;
            back_sentinel->prev = to_delete->prev;
            delete to_delete;
            --deque_size;
        }

        void pop_front() {
            if (empty()) throw std::out_of_range("Deque is empty");
            Node* to_delete = front_sentinel->next;
            to_delete->next->prev = front_sentinel;
            front_sentinel->next = to_delete->next;
            delete to_delete;
            --deque_size;
        }

        T& front() {
            if (empty()) throw std::out_of_range("Deque is empty");
            return front_sentinel->next->data;
        }

        const T& front() const {
            if (empty()) throw std::out_of_range("Deque is empty");
            return front_sentinel->next->data;
        }

        T& back() {
            if (empty()) throw std::out_of_range("Deque is empty");
            return back_sentinel->prev->data;
        }

        const T& back() const {
            if (empty()) throw std::out_of_range("Deque is empty");
            return back_sentinel->prev->data;
        }

        size_t size() const { return deque_size; }
        bool empty() const { return deque_size == 0; }
        size_t max_size() const { return std::numeric_limits<size_t>::max(); }

        class iterator;
        class const_iterator;

        iterator begin() { return iterator(front_sentinel->next); }
        iterator end() { return iterator(back_sentinel); }
        const_iterator begin() const { return const_iterator(front_sentinel->next); }
        const_iterator end() const { return const_iterator(back_sentinel); }
        const_iterator cbegin() const { return const_iterator(front_sentinel->next); }
        const_iterator cend() const { return const_iterator(back_sentinel); }

        bool operator==(const Deque& other) const {
            return std::equal(cbegin(), cend(), other.cbegin(), other.cend());
        }

        bool operator!=(const Deque& other) const { return !(*this == other); }
    };

    template <typename T>
    class Deque<T>::iterator {
        Node* current;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(Node* node = nullptr) : current(node) {}

        reference operator*() { return current->data; }
        pointer operator->() { return &current->data; }

        iterator& operator++() { current = current->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        iterator& operator--() { current = current->prev; return *this; }
        iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    template <typename T>
    class Deque<T>::const_iterator {
        const Node* current;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const Node* node = nullptr) : current(node) {}
        const_iterator(iterator it) : current(it.current) {}

        reference operator*() { return current->data; }
        pointer operator->() { return &current->data; }

        const_iterator& operator++() { current = current->next; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        const_iterator& operator--() { current = current->prev; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }

        bool operator==(const const_iterator& other) const { return current == other.current; }
        bool operator!=(const const_iterator& other) const { return current != other.current; }
    };

}

#endif //FUND_ALG_DEQUE_HPP
