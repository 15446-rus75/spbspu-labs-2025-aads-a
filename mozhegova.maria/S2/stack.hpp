#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <stdexcept>

namespace mozhegova
{
  template< typename T >
  class Stack
  {
  public:
    Stack();
    Stack(const Stack< T > &);
    Stack(Stack< T > &&);
    ~Stack();

    Stack< T > & operator=(const Stack< T > &);
    Stack< T > & operator=(Stack< T > &&);

    void push(const T & value);
    void pop();
    T & top();
    const T & top() const;

    bool empty() const noexcept;
    size_t size() const noexcept;
  private:
    size_t size_;
    size_t capacity_;
    T * data_;

    void resize();
  };

  template< typename T >
  Stack< T >::Stack():
    size_(0),
    capacity_(50),
    data_(new T[capacity_])
  {}

  template< typename T >
  Stack< T >::Stack(const Stack< T > & other):
    size_(other.size_),
    capacity_(other.capacity_),
    data_(new T[capacity_])
  {
    for (size_t i = 0; i < size_; i++)
    {
      data_[i] = other.data_[i];
    }
  }

  template< typename T >
  Stack< T >::Stack(Stack< T > && other):
    size_(other.size_),
    capacity_(other.capacity_),
    data_(other.data_)
  {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  template< typename T >
  Stack< T >::~Stack()
  {
    delete[] data_;
  }

  template< typename T >
  Stack< T > & Stack< T >::operator=(const Stack< T > & other)
  {
    if (this != &other)
    {
      Stack< T > copy(other);
      std::swap(size_, copy.size_);
      std::swap(capacity_, copy.capacity_);
      std::swap(data_, copy.data_);
    }
    return *this;
  }

  template< typename T >
  Stack< T > & Stack< T >::operator=(Stack< T > && other)
  {
    if (this != &other)
    {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(data_, other.data_);
      delete[] other.data_;
    }
    return *this;
  }

  template< typename T >
  void Stack< T >::push(const T & value)
  {
    if (size_ == capacity_)
    {
      resize();
    }
    data_[size_++] = value;
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (empty())
    {
      throw std::logic_error("empty stack");
    }
    size_--;
  }

  template< typename T >
  T & Stack< T >::top()
  {
    if (empty())
    {
      throw std::logic_error("empty stack");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  const T & Stack< T >::top() const
  {
    if (empty())
    {
      throw std::logic_error("empty stack");
    }
    return data_[size_ - 1];
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  void Stack< T >::resize()
  {
    capacity_ *= 2;
    T * temp = new T[capacity_];
    for (size_t i = 0; i < size_; i++)
    {
      temp[i] = data_[i];
    }
    delete[] data_;
    data_ = temp;
  }
}

#endif
