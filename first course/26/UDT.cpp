#include <iostream>
#include <algorithm>
#include "UDT.h"

Udt::Udt(int len) {
    this->depth = 0;
    this->len = len;
    this->mas = new int[len];
}

Udt::Udt(const Udt& other) {
    this->depth = other.depth;
    this->len = other.len;
    this->mas = new int[this->len];
    for (int i = 0; i < this->depth; ++i) {
        this->mas[i] = other.mas[i];
    }
}

Udt::~Udt() {
    delete[] this->mas;
}

void Udt::resize(int size) {
    this->len += size;
    int* temp =  new int[this->len];
    for (int i = 0; i < this->depth - 1; ++i) {
        temp[i] = this->mas[i];
    }
    delete[] this->mas;
    this->mas = new int[this->len];

    for (int i = 0; i < this->depth - 1; ++i) {
        this->mas[i] = temp[i];
    }
    delete[] temp;
}

bool Udt::is_empty() const {
    return this->depth == 0;
}

void Udt::push_front(int value) {
    if (this->depth == this->len) {
        this->resize(1);
    }
    ++this->depth;
    for (int i = this->depth - 1; i > 0; --i) {
        this->mas[i] = this->mas[i - 1];
    }
    this->mas[0] = value;
}

void Udt::push_back(int value) {
    if (this->depth == this->len) {
        this->resize(1);
    }
    this->mas[this->depth] = value;
    ++this->depth;
}

void Udt::pop_front() {
    for (int i = 1; i < this->depth - 1; ++i) {
        this->mas[i - 1] = this->mas[i];
    }
    --this->depth;
}

void Udt::pop_back() {
    --this->depth;
    this->mas[this->depth] = 0;
}

void Udt::display() const {
    for (int i = 0; i < this->depth; ++i) {
        std::cout << this->mas[i] << ' ';
    }
    std::cout << std::endl;
}

int Udt::size() const{
    return this->depth;
}

void Udt::insert_sort() {
    for (int i = 0; i < this->depth; ++i) {
        int min_i = i;
        for (int j = i + 1; j < this->depth; ++j) {
            if (this->mas[min_i] > this->mas[j]) {
                min_i = j;
            }
        }
        std::swap(this->mas[i], this->mas[min_i]);
    }
}

void Udt::delete_value(int value) {
    int i;
    for (i = 0; i < this->depth; ++i) {
        if (this->mas[i] == value) {
            break;
        }
    }

    if (i < this->depth) {
        --this->depth;
        for (int j = i; j < this->depth; ++j) {
            this->mas[j] = this->mas[j + 1];
        }
    }
}

void Udt::max_el_del() {
    Udt temp = *this;
    temp.insert_sort();
    int max_el = temp.mas[this->depth - 1];
    this->delete_value(max_el);
};

Udt& Udt::operator=(const Udt& other) {
    if (this != &other) {
        delete[] this->mas;
        this->depth = other.depth;
        this->len = other.len;
        this->mas = new int[this->len];
        for (int i = 0; i < this->depth; ++i) {
            this->mas[i] = other.mas[i];
        }
    }
    return *this;
}

