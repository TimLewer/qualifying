#include <cstddef>
#include <memory>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <type_traits>


template <typename T>
class Vector {
public:
//Construct
    Vector();
    Vector(const Vector& other);
    Vector(Vector&& other);
    Vector(std::initializer_list<T> list);

    ~Vector();

//Assignment
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    Vector& operator=(std::initializer_list<T> list);

//Memory
    void Resize(size_t new_size = 0, T element = T());
    void Reserve(size_t new_size);

//AccessingElements
    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;

    T& At(size_t idx);
    const T& At(size_t idx) const;

    T& Front();
    const T& Front() const;

    T& Back();
    const T& Back() const;

//Modification
    void Push_back(T element);
    void Pop_back();
    void Insert(size_t idx, T element);
    void Erase(size_t idx);
    void Clear();

//Info
    size_t Size() const;
    size_t Capacity() const;
    bool Empty() const;

private:
    std::unique_ptr<T[]> buf_;
    size_t size_;
    size_t capacity_;

    void Allocation();
};

//Realisation

template<typename T>
Vector<T>::Vector() {
    buf_ = std::make_unique<T[]>(2);
    size_ = 0;
    capacity_ = 2;
}

template<typename T>
Vector<T>::Vector(const Vector& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    buf_ = std::make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; ++i) {
        buf_[i] = other.buf_[i];
    }
}

template <typename T>
Vector<T>::Vector(Vector&& other) {
    buf_ = std::move(other.buf_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
    other.size_ = 0;
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> list) {
    size_ = list.size();
    capacity_ = list.size();
    buf_ = std::make_unique<T[]>(capacity_);
    size_t i = 0;
    for (const T& val : list) {
        buf_[i++] = val;
    }
}

template<typename T>
Vector<T>::~Vector() = default;

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        Clear();
        size_ = other.size_;
        capacity_ = other.capacity_;
        buf_ = std::make_unique<T[]>(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            buf_[i] = other.buf_[i];
        }
    }
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    buf_ = std::move(other.buf_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.capacity_ = 0;
    other.size_ = 0;
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> list) {
    size_ = list.size();
    capacity_ = list.size();
    buf_ = std::make_unique<T[]>(capacity_);
    size_t i = 0;
    for (const T& val : list) {
        buf_[i++] = val;
    }
    return *this;
}

template<typename T>
void Vector<T>::Resize(size_t new_size, T element) {
    if (new_size > capacity_) {
        Reserve(new_size);
    }
    if (new_size > size_) {
        for (size_t i = size_; i < new_size; ++i) {
            buf_[i] = element;
        }
    } else {
        for (size_t i = new_size; i < size_; ++i) {
            buf_[i].~T();
        }
    }
    size_ = new_size;
}

template<typename T>
void Vector<T>::Reserve(size_t new_capacity) {
    if (new_capacity <= capacity_) return;

    std::unique_ptr<T[]> new_buf = std::make_unique<T[]>(new_capacity);
    for (size_t i = 0; i < size_; ++i) {
        new_buf[i] = std::move(buf_[i]);
    }
    buf_ = std::move(new_buf);
    capacity_ = new_capacity;
}

template <typename T>
T& Vector<T>::operator[](size_t idx) {
    return buf_[idx];
}

template <typename T>
const T& Vector<T>::operator[](size_t idx) const {
    return buf_[idx];
}

template<typename T>
T& Vector<T>::At(size_t idx) {
    if (idx < size_) {
        return buf_[idx];
    }
    throw std::out_of_range("Index out of range");
}

template<typename T>
const T& Vector<T>::At(size_t idx) const {
    if (idx < size_) {
        return buf_[idx];
    }
    throw std::out_of_range("Index out of range");
}

template<typename T>
T& Vector<T>::Front() {
    return buf_[0];
}

template<typename T>
const T& Vector<T>::Front() const {
    return buf_[0];
}

template<typename T>
T& Vector<T>::Back() {
    return buf_[size_ - 1];
}

template<typename T>
const T& Vector<T>::Back() const {
    return buf_[size_ - 1];
}

template<typename T>
void Vector<T>::Allocation() {
    size_t new_capacity = capacity_ * 2;
    std::unique_ptr<T[]> buf_new = std::make_unique<T[]>(new_capacity);
    for (size_t i = 0; i < size_; ++i) {
        buf_new[i] = std::move(buf_[i]);
    }
    capacity_ = new_capacity;
    buf_ = std::move(buf_new);
}

template<typename T>
void Vector<T>::Push_back(T element) {
    if (size_ == capacity_) {
        Allocation();
    }
    buf_[size_] = element;
    ++size_;
}

template<typename T>
void Vector<T>::Pop_back() {
    if (!Empty()) {
        --size_;
    }
}

template<typename T>
void Vector<T>::Insert(size_t idx, T element) {
    if (idx > size_) return;
    if (size_ + 1 > capacity_) {
        Allocation();
    }
    for (size_t i = size_; i > idx; --i) {
        buf_[i] = std::move(buf_[i - 1]);
    }
    buf_[idx] = element;
    ++size_; 
}

template<typename T>
void Vector<T>::Erase(size_t idx) {
    if (idx >= size_) return;

    for (size_t i = idx; i < size_ - 1; ++i) {
        buf_[i] = std::move(buf_[i + 1]); 
    }
    --size_;
}

template<typename T>
void Vector<T>::Clear() {
    if (!std::is_trivial_v<T>) {
        for (size_t i = 0; i < size_; ++i) {
            buf_[i].~T();
        }
    }
    size_ = 0;
}

template<typename T>
size_t Vector<T>::Size() const {
    return size_;
}

template<typename T>
size_t Vector<T>::Capacity() const {
    return capacity_;
}

template <typename T>
bool Vector<T>::Empty() const {
    return size_ == 0;
}