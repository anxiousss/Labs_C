
#ifndef FUND_ALG_VECTOR_HPP
#define FUND_ALG_VECTOR_HPP


#include "Container.hpp"
#include <limits>
#include <stdexcept>

namespace my_container {

    template <typename T>
    class Vector : public Container<T> {


    public:
        using iterator = typename Container<T>::iterator;
        using const_iterator = typename Container<T>::const_iterator;

        Vector();

        Vector(const Vector& other);

        Vector(Vector&& other) noexcept;

        Vector(std::initializer_list<T> init_list);

        Vector& operator=(std::initializer_list<T> init_list);

        Vector& operator=(Vector&& other) noexcept;

        Vector& operator=(const Vector& other);

        ~Vector() override;

        bool operator==(const Container<T>& other) const override;

        bool operator!=(const Container<T>& other) const override;

        bool operator==(const Vector<T>& other) const;

        bool operator!=(const Vector<T>& other) const;

        size_t size() const override;

        size_t max_size() const noexcept override;

        bool empty() const override;

        void push_back(const T& value);

        void push_back(T&& value);

        void pop_back();

        T& operator[](size_t index);

        const T& operator[](size_t index) const;

        T& at(size_t index);

        const T& at(size_t index) const;

        T& front();

        const T& front() const;

        T& back();

        const T& back() const;

        T* data() noexcept;

        const T* data() const noexcept;

        iterator begin();

        iterator end();

        const_iterator begin() const;

        const_iterator end() const;

        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        void resize(size_t new_size, const T& value = T());

        void reserve(size_t new_capacity);

        size_t capacity() const;

        void shrink_to_fit();

        void clear() noexcept;

        void swap(Vector& other) noexcept;

        iterator insert(const_iterator pos, const T& value);

        iterator insert(const_iterator pos, T&& value);

        iterator erase(const_iterator pos);

        iterator erase(const_iterator first, const_iterator last);

        void resize(size_t new_size);

        auto operator<=>(const Vector& other) const;

    private:
        T* data_;
        size_t size_;
        size_t capacity_;

        void reallocate(size_t new_capacity);
        iterator insert_impl(const_iterator pos, T&& value);
    };

    template<typename T>
    typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept {
        return data_;  // Неявное преобразование T* -> const T*
    }

    template <typename T>
    typename Vector<T>::const_iterator Vector<T>::cend() const noexcept {
        return data_ + size_;
    }
    template <typename T>
    void Vector<T>::resize(size_t new_size, const T& value) {
        if (new_size < size_) {
            // Уменьшаем размер
            while (size_ > new_size) {
                data_[--size_].~T();
            }
        } else {
            // Увеличиваем размер
            reserve(new_size);
            while (size_ < new_size) {
                new (data_ + size_) T(value);  // Placement new с копированием значения
                ++size_;
            }
        }
    }


    template<typename T>
    Vector<T>& Vector<T>::operator=(std::initializer_list<T> init_list) {
        clear();
        reserve(init_list.size());
        for (const auto& item : init_list) {
            new(data_ + size_) T(item);  // Placement new вместо push_back
            ++size_;
        }
        return *this;
    }

    template<typename T>
    Vector<T>::Vector(std::initializer_list<T> init_list)
        : data_(nullptr), size_(0), capacity_(0)
        {
            reserve(init_list.size());
            for (const auto& item : init_list) {
                new (data_ + size_) T(item); // Placement new
                ++size_;
            }
    }

    template<typename T>
    void Vector<T>::resize(size_t new_size) {
        if (new_size < size_) {
            while (size_ > new_size) {
                data_[--size_].~T();  // Явный вызов деструктора
            }
        } else {
            reserve(new_size);
            while (size_ < new_size) {
                new (data_ + size_) T();  // Placement new
                ++size_;
            }
        }
    }

    template<typename T>
    Vector<T>::iterator Vector<T>::erase(Vector::const_iterator first, Vector::const_iterator last) {
        if(first >= last) return begin();

        size_t start = first - begin();
        size_t count = last - first;

        for(size_t i = start; i < size_ - count; ++i) {
            data_[i] = std::move(data_[i + count]);
        }
        size_ -= count;
        return begin() + start;
    }


    template<typename T>
    Vector<T>::iterator Vector<T>::erase(Vector::const_iterator pos) {
        return erase(pos, pos + 1);
    }

    template<typename T>
    Vector<T>::iterator Vector<T>::insert(Vector::const_iterator pos, T &&value) {
        return insert_impl(pos, std::move(value));
    }

    template<typename T>
    Vector<T> ::iterator Vector<T>::insert(Vector::const_iterator pos, const T &value) {
        return insert_impl(pos, T(value));
    }

    template<typename T>
    Vector<T>::iterator Vector<T>::insert_impl(Vector::const_iterator pos, T &&value) {
        size_t offset = pos - begin();
        if(size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }

        for(size_t i = size_; i > offset; --i) {
            data_[i] = std::move(data_[i-1]);
        }
        data_[offset] = std::forward<T>(value);
        ++size_;
        return begin() + offset;
    }


    template<typename T>
    auto Vector<T>::operator<=>(const Vector<T> &other) const{
        if (auto cmp = size_ <=> other.size_; cmp != 0) {
            return cmp;
        }

        // 2. Только если размеры равны, сравниваем элементы
        return std::lexicographical_compare_three_way(
                data_,
                data_ + size_,
                other.data_,
                other.data_ + other.size_
        );
    }

    template<typename T>
    bool Vector<T>::operator==(const Container<T>& other) const  {
        const Vector<T>* other_vec = dynamic_cast<const Vector<T>*>(&other);
        return other_vec && (*this == *other_vec);
    }

    template<typename T>
    bool Vector<T>::operator!=(const Container<T>& other) const {
        return !(*this == other);
    }

    template<typename T>
    bool Vector<T>::operator==(const Vector<T>& other) const {
            return (*this <=> other) == 0;
        }

    template<typename T>
    bool Vector<T>::operator!=(const Vector<T>& other) const {
        return !(*this == other);
    }

    template<typename T>
    void Vector<T>::swap(Vector &other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

    template<typename T>
    void Vector<T>::clear() noexcept {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    template<typename T>
    void Vector<T>::shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_ > 0 ? size_ : 0); // Обработка пустого вектора
        }
    }

    template<typename T>
    void Vector<T>::reallocate(size_t new_capacity) {
        if (new_capacity == 0) {
            clear();
            return;
        }

        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new(new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }

        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;

    }

    template<typename T>
    const T *Vector<T>::data() const noexcept { return data_; }

    template<typename T>
    T *Vector<T>::data() noexcept { return data_; }

    template<typename T>
    const T &Vector<T>::back() const {
        if(empty()) throw std::out_of_range("Vector is empty");
        return data_[size_-1];
    }

    template<typename T>
    T &Vector<T>::back() {
        if(empty()) throw std::out_of_range("Vector is empty");
        return data_[size_-1];
    }

    template<typename T>
    const T &Vector<T>::front() const {
        if(empty()) throw std::out_of_range("Vector is empty");
        return data_[0];
    }

    template<typename T>
    T &Vector<T>::front() {
        if(empty()) throw std::out_of_range("Vector is empty");
        return data_[0];
    }

    template<typename T>
    const T &Vector<T>::at(size_t index) const {
        if(index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    template<typename T>
    T &Vector<T>::at(size_t index) {
        if(index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    template<typename T>
    void Vector<T>::push_back(T &&value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {
        if (this != &other) {
            // Уничтожаем старые элементы
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            ::operator delete(data_);

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;  // Обнуляем capacity
        }
        return *this;
    }

    template<typename T>
    Vector<T>::Vector(Vector &&other) noexcept
            : data_(other.data_),
              size_(other.size_),
              capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    template<typename T>
    const T &Vector<T>::operator[](size_t index) const {
        if (index >= size_ || !data_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    template<typename T>
    T &Vector<T>::operator[](size_t index) {
        if (index >= size_  || !data_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    template<typename T>
    void Vector<T>::pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    template<typename T>
    size_t Vector<T>::capacity() const {
        return capacity_;
    }

    template<typename T>
    void Vector<T>::reserve(size_t new_capacity) {
        if(new_capacity <= capacity_) return;
        reallocate(new_capacity);
    }

    template<typename T>
    Vector<T>::const_iterator Vector<T>::end() const {
        return data_ + size_;
    }

    template<typename T>
    Vector<T>::const_iterator Vector<T>::begin() const {
        return data_;
    }

    template<typename T>
    Vector<T>::iterator Vector<T>::end() {
        return data_ + size_;
    }

    template<typename T>
    Vector<T>::iterator Vector<T>::begin() {
        return data_;
    }

    template<typename T>
    void Vector<T>::push_back(const T &value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : 2 * capacity_);
        }
        new (data_ + size_) T(value);
        ++size_;
    }

    template<typename T>
    bool Vector<T>::empty() const {
        return size_ == 0;
    }

    template<typename T>
    size_t Vector<T>::max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }

    template<typename T>
    size_t Vector<T>::size() const {
        return size_;
    }

    template<typename T>
    Vector<T>::~Vector() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        ::operator delete(data_);
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(const Vector &other) {
        if (this != &other) {
            // Уничтожаем старые элементы
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            ::operator delete(data_);

            // Копируем новые данные
            size_ = other.size_;
            capacity_ = other.size_;
            if (capacity_ > 0) {
                data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
                for (size_t i = 0; i < size_; ++i) {
                    new(data_ + i) T(other.data_[i]);
                }
            } else {
                data_ = nullptr;
            }
        }
        return *this;
    }

    template<typename T>
    Vector<T>::Vector(const Vector& other)
            : data_(nullptr), size_(other.size_), capacity_(other.size_)  // capacity = size
    {
        if (capacity_ > 0) {
            data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
            for (size_t i = 0; i < size_; ++i) {
                new(data_ + i) T(other.data_[i]);  // Placement new вместо присваивания
            }
        }
    }

    template<typename T>
    Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

} // namespace my_container

#endif //FUND_ALG_VECTOR_HPP
