#ifndef ARRAY_BUFFER_HPP
#define ARRAY_BUFFER_HPP

#include <cstddef>
#include <stdexcept>
#include <iostream>

namespace gavrilova
{
  template <typename T>
  class ArrayBuffer
  {
  public:
    ArrayBuffer();
    ArrayBuffer(const ArrayBuffer& other);
    ArrayBuffer(ArrayBuffer&& other) noexcept;
    ~ArrayBuffer();

    ArrayBuffer& operator=(const ArrayBuffer& other);
    ArrayBuffer& operator=(ArrayBuffer&& other) noexcept;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void clear();

    bool empty() const;
    size_t size() const;
    size_t capacity() const;

    void swap(ArrayBuffer& other) noexcept;

  private:
    size_t size_;
    size_t capacity_;
    T* data_;

    static constexpr size_t DEFAULT_CAPACITY = 10;
    static constexpr size_t KOEF_FOR_GROWTH = 2;

    void resize(size_t new_capacity);
  };

  template <typename T>
  ArrayBuffer<T>::ArrayBuffer()
  {
    size_ = 0;
    capacity_ = DEFAULT_CAPACITY;
    data_ = new T[capacity_];
  }

  template <typename T>
  ArrayBuffer<T>::ArrayBuffer(const ArrayBuffer& other)
  {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
      try {
        data_[i] = other.data_[i];
      } catch (const std::bad_alloc&) {
        delete[] data_;
        throw;
      }
    }
  }

  template <typename T>
  ArrayBuffer<T>::ArrayBuffer(ArrayBuffer&& other) noexcept
  {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = other.data_;
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  template <typename T>
  ArrayBuffer<T>::~ArrayBuffer()
  {
    delete[] data_;
  }

  template <typename T>
  ArrayBuffer<T>& ArrayBuffer<T>::operator=(const ArrayBuffer& other)
  {
    if (this != std::addressof(other)) {
      ArrayBuffer<T> cpy(other);
      swap(cpy);
    }
    return *this;
  }

  template <typename T>
  ArrayBuffer<T>& ArrayBuffer<T>::operator=(ArrayBuffer&& other) noexcept
  {
    if (this != std::addressof(other)) {
      ArrayBuffer<T> cpy(std::move(other));
      swap(cpy);
    }
    return *this;
  }

  template <typename T>
  T& ArrayBuffer<T>::operator[](size_t index)
  {
    if (index >= size_) throw std::out_of_range("Index out of range");
    return data_[index];
  }

  template <typename T>
  const T& ArrayBuffer<T>::operator[](size_t index) const
  {
    return data_[index];
  }

  template <typename T>
  T& ArrayBuffer<T>::front()
  {
    if (empty()) {
      throw std::out_of_range("Buffer is empty");
    }
    return data_[0];
  }

  template <typename T>
  const T& ArrayBuffer<T>::front() const
  {
    if (empty()) {
      throw std::out_of_range("Buffer is empty");
    }
    return data_[0];
  }

  template <typename T>
  T& ArrayBuffer<T>::back()
  {
    if (empty()) {
      throw std::out_of_range("Buffer is empty");
    }
    return data_[size_ - 1];
  }

  template <typename T>
  const T& ArrayBuffer<T>::back() const
  {
    if (empty()) {
      throw std::out_of_range("Buffer is empty");
    }
    return data_[size_ - 1];
  }

  template <typename T>
  void ArrayBuffer<T>::push_back(const T& value)
  {
    if (capacity_ == 0) {
      resize(DEFAULT_CAPACITY);
    }
    else if (size_ == capacity_) {
      resize(capacity_ * KOEF_FOR_GROWTH);
    }
    data_[size_++] = value;
  }

  template <typename T>
  void ArrayBuffer<T>::push_back(T&& value)
  {
    if (capacity_ == 0) {
      resize(DEFAULT_CAPACITY);
    }
    else if (size_ == capacity_) {
      resize(capacity_ * KOEF_FOR_GROWTH);
    }
    data_[size_++] = std::move(value);
  }

  template <typename T>
  void ArrayBuffer<T>::pop_back()
  {
    if (empty()) {
      throw std::out_of_range("Buffer is empty");
    }
    --size_;
  }

  template <typename T>
  void ArrayBuffer<T>::clear()
  {
    size_ = 0;
  }

  template <typename T>
  bool ArrayBuffer<T>::empty() const
  {
    return size_ == 0;
  }

  template <typename T>
  size_t ArrayBuffer<T>::size() const
  {
    return size_;
  }

  template <typename T>
  size_t ArrayBuffer<T>::capacity() const
  {
    return capacity_;
  }

  template <typename T>
  void ArrayBuffer<T>::swap(ArrayBuffer& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
  }

  template <typename T>
  void ArrayBuffer<T>::resize(size_t new_capacity)
  {
    T* new_data = new T[new_capacity];
    for (size_t i = 0; i < size_; ++i) {
      try {
        new_data[i] = data_[i];
      } catch (const std::bad_alloc&) {
        delete[] new_data;
        throw;
      }
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }

}

#endif
