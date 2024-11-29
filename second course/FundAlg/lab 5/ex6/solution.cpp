#include "solution.h"

Vector::Vector() : data_(nullptr), size_(0), capacity_(0) {}

Vector::Vector(size_t n, const double& default_value)
        : size_(n), capacity_(n) {
    try {
        data_ = new double[n];
    } catch (const std::bad_alloc& e) {
        throw e;
    }
    for (size_t i = 0; i < n; ++i) {
        data_[i] = default_value;
    }
}

Vector::Vector(const Iterator& first, const Iterator& last)
        : size_(0), capacity_(0), data_(nullptr) {
    for (Iterator it = first; it != last; ++it) {
        push_back(*it);
    }
}

Vector::Vector(const std::initializer_list<double> &il) : size_(0), capacity_(il.size() * 2) {
    try {
        data_ = new  double[capacity_];
    } catch (const std::bad_alloc& e) {
        throw e;
    }
    reserve(il.size() * 2);
    auto begin = il.begin();
    auto end = il.end();
    for (auto i = begin;  i != end ; ++i) {
        push_back(*i);
    }
}



Vector::~Vector() {
    delete[] data_;
}

double& Vector::at(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

const double& Vector::at(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

double& Vector::front() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[0];
}

const double& Vector::front() const {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[0];
}

double& Vector::back() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[size_ - 1];
}

const double& Vector::back() const {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[size_ - 1];
}

double* Vector::data() {
    return data_;
}

const double* Vector::data() const {
    return data_;
}

bool Vector::empty() const {
    return size_ == 0;
}

size_t Vector::size() const {
    return size_;
}

size_t Vector::capacity() const {
    return capacity_;
}

void Vector::reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        double* new_data;
        try {
            new_data = new double[new_capacity];
        } catch (const std::bad_alloc& e) {
            throw e;
        }
        if (data_) {
            std::copy(data_, data_ + size_, new_data);
            delete[] data_;
        }
        data_ = new_data;
        capacity_ = new_capacity;
    }
}

void Vector::shrink_to_fit() {
    if (capacity_ > size_) {
        double* new_data;
        try {
            new_data = new double[size_];
        } catch (const std::bad_alloc& e) {
            throw e;
        }
        std::copy(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
        capacity_ = size_;
    }
}

void Vector::clear() {
    size_ = 0;
}

void Vector::insert(size_t index, const double& elem) {
    if (index > size_) {
        throw std::out_of_range("Index out of range");
    }
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    for (size_t i = size_; i > index; --i) {
        data_[i] = data_[i - 1];
    }
    data_[index] = elem;
    ++size_;
}

void Vector::erase(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    for (size_t i = index; i < size_ - 1; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
}

void Vector::push_back(const double& elem) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_] = elem;
    ++size_;
}

void Vector::pop_back() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    --size_;
}

void Vector::resize(size_t new_size, const double& elem) {
    if (new_size > capacity_) {
        reserve(new_size);
    }
    if (new_size > size_) {
        for (size_t i = size_; i < new_size; ++i) {
            data_[i] = elem;
        }
    }
    size_ = new_size;
}

auto Vector::operator<=>(const Vector& other) const {
    if (size_ != other.size_) {
        if (size_ < other.size_) return -1;
        else return 1;
    }
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i] < other.data_[i]) return -1;
        if (data_[i] > other.data_[i]) return 1;
    }
    return 0;
}

bool Vector::operator==(const Vector& other) const {
    if (size_ != other.size_) return false;
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) return false;
    }
    return true;
}

Vector::Iterator Vector::begin() {
    return {data_};
}

Vector::Iterator Vector::end() {
    return {data_ + size_};
}

Vector::Iterator::Iterator(double* ptr) : ptr_(ptr) {}

double& Vector::Iterator::operator*() const {
    return *ptr_;
}

double* Vector::Iterator::operator->() const {
    return ptr_;
}

Vector::Iterator& Vector::Iterator::operator++() {
    ++ptr_;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
    Iterator temp = *this;
    ++ptr_;
    return temp;
}

Vector::Iterator& Vector::Iterator::operator--() {
    --ptr_;
    return *this;
}

Vector::Iterator Vector::Iterator::operator--(int) {
    Iterator temp = *this;
    --ptr_;
    return temp;
}

Vector::Iterator Vector::Iterator::operator+(ptrdiff_t n) const {
    return {ptr_ + n};
}

Vector::Iterator Vector::Iterator::operator-(ptrdiff_t n) const {
    return {ptr_ - n};
}

ptrdiff_t Vector::Iterator::operator-(const Iterator& other) const {
    return ptr_ - other.ptr_;
}

double& Vector::Iterator::operator[](ptrdiff_t n) const {
    return ptr_[n];
}

bool Vector::Iterator::operator==(const Iterator& other) const {
    return ptr_ == other.ptr_;
}

bool Vector::Iterator::operator!=(const Iterator& other) const {
    return ptr_ != other.ptr_;
}

bool Vector::Iterator::operator<(const Iterator& other) const {
    return ptr_ < other.ptr_;
}

bool Vector::Iterator::operator>(const Iterator& other) const {
    return ptr_ > other.ptr_;
}

bool Vector::Iterator::operator<=(const Iterator& other) const {
    return ptr_ <= other.ptr_;
}

bool Vector::Iterator::operator>=(const Iterator& other) const {
    return ptr_ >= other.ptr_;
}

std::ostream& operator<<(std::ostream& stream, const Vector& v){
    if (v.empty()){
        stream << "Capacity = " << v.capacity();
        stream << "Vector is Empty\n";
        return stream;
    }
    stream << "Capacity = " << v.capacity() << std::endl;
    stream << "Size = " << v.size() << std::endl;
    for (size_t i = 0; i < v.size(); i++){
        double y = v.at(i);
        stream << y << " ";
    }
    stream << std::endl;
    return stream;
}