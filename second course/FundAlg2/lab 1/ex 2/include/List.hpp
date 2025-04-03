#ifndef FUND_ALG_LIST_HPP
#define FUND_ALG_LIST_HPP

#pragma once

#include <initializer_list>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include "Container.hpp"

namespace my_container {


    template <typename T>
    class List : public Container<T> {
    private:
        struct Node {
            T data;
            Node* prev;
            Node* next;

            explicit Node(const T& data = T(), Node* prev = nullptr, Node* next = nullptr)
                    : data(data), prev(prev), next(next) {}
        };

        Node* sentinel;
        size_t list_size;

        void copyNodes(const List& other) {
            Node* other_current = other.sentinel->next;
            while (other_current != other.sentinel) {
                push_back(other_current->data);
                other_current = other_current->next;
            }
        }

    public:
        List();

        List(std::initializer_list<T> init);

        List(const List& other);

        List(List&& other) noexcept;

        List& operator=(const List& other);

        List& operator=(List&& other) noexcept;

        ~List() override;

        T& front();

        const T& front() const;

        T& back();

        const T& back() const;

        bool operator==(const Container<T>& other) const override;
        bool operator!=(const Container<T>& other) const override;
        bool operator<(const List& other) const;

        bool empty() const override { return list_size == 0; }
        size_t size() const override { return list_size; }
        size_t max_size() const noexcept override{ return std::numeric_limits<size_t>::max(); }

        void clear();

        class iterator {

        public:
            Node* current;
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            iterator(Node* node = nullptr) : current(node) {}

            reference operator*() const { return current->data; }
            pointer operator->() const { return &current->data; }

            iterator& operator++() { current = current->next; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
            iterator& operator--() { current = current->prev; return *this; }
            iterator operator--(int) { iterator tmp = *this; --(*this); return tmp; }

            bool operator==(const iterator& other) const  { return current == other.current; }
            bool operator!=(const iterator& other) const { return !(*this == other); }
        };

        class const_iterator {

        public:
            const Node* current;
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = const T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            const_iterator(const Node* node = nullptr) : current(node) {}
            const_iterator(const iterator& it) : current(it.current) {}

            reference operator*() const { return current->data; }
            pointer operator->() const { return &current->data; }

            const_iterator& operator++() { current = current->next; return *this; }
            const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
            const_iterator& operator--() { current = current->prev; return *this; }
            const_iterator operator--(int) { const_iterator tmp = *this; --(*this); return tmp; }

            bool operator==(const const_iterator& other) const { return current == other.current; }
            bool operator!=(const const_iterator& other) const { return !(*this == other); }
        };

        iterator begin() noexcept { return iterator(sentinel->next); }
        iterator end() noexcept { return iterator(sentinel); }
        const_iterator cbegin() const noexcept { return const_iterator(sentinel->next); }
        const_iterator cend() const noexcept { return const_iterator(sentinel); }



        iterator insert(const_iterator pos, const T& value);

        iterator erase(const_iterator pos);

        void push_back(const T& value) { insert(end(), value); }
        void pop_back() { erase(--end()); }
        void push_front(const T& value) { insert(begin(), value); }
        void pop_front() { erase(begin()); }

        void resize(size_t count, const T& value = T());

        void swap(List& other) noexcept;


    };
    template<typename T>
    bool List<T>::operator==(const Container<T>& other) const {
        const List<T>* otherList = dynamic_cast<const List<T>*>(&other);
        if (!otherList || list_size != otherList->list_size) return false;
        return std::equal(cbegin(), cend(), otherList->cbegin());
    }

    template<typename T>
    bool List<T>::operator!=(const Container<T>& other) const {
        return !(*this == other);
    }

    template<typename T>
    bool List<T>::operator<(const List& other) const {
        return std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
    }

    template<typename T>
    List<T>::iterator List<T>::erase(List::const_iterator pos) {
        Node* next_node = pos.current->next;
        pos.current->prev->next = pos.current->next;
        pos.current->next->prev = pos.current->prev;
        delete pos.current;
        list_size--;
        return iterator(next_node);
    }

    template<typename T>
    void List<T>::swap(List &other) noexcept {
        std::swap(sentinel, other.sentinel);
        std::swap(list_size, other.list_size);
    }

    template<typename T>
    void List<T>::resize(size_t count, const T &value) {
        while (list_size > count) pop_back();
        while (list_size < count) push_back(value);
    }

    template<typename T>
    List<T>::iterator List<T>::insert(List::const_iterator pos, const T &value) {
        Node* prev_node = const_cast<Node*>(pos.current->prev);
        Node* next_node = const_cast<Node*>(pos.current);

        Node* new_node = new Node(value, prev_node, next_node);
        prev_node->next = new_node;
        next_node->prev = new_node;
        list_size++;
        return iterator(new_node);
    }

    template<typename T>
    void List<T>::
            clear() {
        while (!empty()) pop_back();
    }

    template<typename T>
    const T &List<T>::back() const {
        if (empty()) throw std::out_of_range("List is empty");
        return sentinel->prev->data;
    }

    template<typename T>
    T &List<T>::back() {
        if (empty()) throw std::out_of_range("List is empty");
        return sentinel->prev->data;
    }

    template<typename T>
    const T &List<T>::front() const {
        if (empty()) throw std::out_of_range("List is empty");
        return sentinel->next->data;
    }

    template<typename T>
    T &List<T>::front() {
        if (empty()) throw std::out_of_range("List is empty");
        return sentinel->next->data;
    }

    template<typename T>
    List<T>::~List() {
        clear();
        delete sentinel;
    }

    template<typename T>
    List<T> &List<T>::operator=(List &&other) noexcept {
        if (this != &other) {
            clear();
            delete sentinel;

            sentinel = other.sentinel;
            list_size = other.list_size;

            other.sentinel = new Node();
            other.sentinel->prev = other.sentinel->next = other.sentinel;
            other.list_size = 0;
        }
        return *this;
    }

    template<typename T>
    List<T> &List<T>::operator=(const List &other) {
        if (this != &other) {
            clear();
            copyNodes(other);
        }
        return *this;
    }

    template<typename T>
    List<T>::List(List &&other) noexcept : sentinel(other.sentinel), list_size(other.list_size) {
        other.sentinel = new Node();
        other.sentinel->prev = other.sentinel->next = other.sentinel;
        other.list_size = 0;
    }

    template<typename T>
    List<T>::List(const List &other) : List() {
        copyNodes(other);
    }

    template<typename T>
    List<T>::List(std::initializer_list<T> init) : List() {
        for (const auto& val : init) push_back(val);
    }

    template<typename T>
    List<T>::List() : list_size(0) {
        sentinel = new Node();
        sentinel->prev = sentinel;
        sentinel->next = sentinel;
    }

}

#endif // FUND_ALG_LIST_HPP