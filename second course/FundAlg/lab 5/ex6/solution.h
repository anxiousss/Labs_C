

#ifndef EX5_6_SOLUTION_H
#define EX5_6_SOLUTION_H

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <iterator>
#include <algorithm>

class Vector {
public:

    class Iterator;

    Iterator begin();
    Iterator end();

    Vector();
    explicit Vector(size_t n, const double& default_value = 0.0);
    Vector(const Iterator& first, const Iterator& last);
    Vector(const std::initializer_list<double>& il);

    ~Vector();

    double& at(size_t index);
    const double& at(size_t index) const;
    double& front();
    const double& front() const;
    double& back();
    const double& back() const;
    double* data();
    const double* data() const;
    bool empty() const;
    size_t size() const;
    size_t capacity() const;

    // Capacity
    void reserve(size_t new_capacity);
    void shrink_to_fit();

    // Modifiers
    void clear();
    void insert(size_t index, const double& elem);
    void erase(size_t index);
    void push_back(const double& elem);
    void pop_back();
    void resize(size_t new_size, const double& elem = double());

    // Comparisons
    auto operator<=>(const Vector& other) const;
    bool operator==(const Vector& other) const;


private:
    double* data_;
    size_t size_;
    size_t capacity_;
};

class Vector::Iterator {
public:
    Iterator(double* ptr);
    double& operator*() const;
    double* operator->() const;
    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);
    Iterator operator+(ptrdiff_t n) const;
    Iterator operator-(ptrdiff_t n) const;
    ptrdiff_t operator-(const Iterator& other) const;
    double& operator[](ptrdiff_t n) const;
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
    bool operator<(const Iterator& other) const;
    bool operator>(const Iterator& other) const;
    bool operator<=(const Iterator& other) const;
    bool operator>=(const Iterator& other) const;


private:
    double* ptr_;
};

std::ostream& operator<<(std::ostream&, const Vector&);

#endif //EX5_6_SOLUTION_H
