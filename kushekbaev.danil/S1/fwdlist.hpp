#ifndef FWDLIST_HPP
#define FWDLIST_HPP

#include <utility>
#include "iterator.hpp"

namespace kushekbaev
{
  template< typename T >
  struct FwdList
  {
    FwdList();
    FwdList(size_t count, const T& value);
    FwdList(std::initializer_list< T > init);
    FwdList(const FwdList& other);
    FwdList(FwdList&& other) noexcept;
    ~FwdList();

    FwdList& operator=(const FwdList& other) noexcept;
    FwdList& operator=(FwdList&& other) noexcept;

    Iterator< T > begin() const noexcept;
    Iterator< T > end() const noexcept;

    Iterator< T > cbegin() const noexcept;
    Iterator< T > cend() const noexcept;

    Iterator< T > before_begin() const noexcept;
    Iterator< T > cbefore_begin() const noexcept;

    T& front() const noexcept;
    T& back() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void assign(size_t size, const T& value);
    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front() noexcept;
    void pop_back() noexcept;
    void swap(FwdList& other) noexcept;
    void clear() noexcept;

    void remove(const T& value);
    template< typename Predicate >
    void remove_if(Predicate predicate);
    void splice_after(Iterator< T > position, FwdList& other);
    void splice_after(Iterator< T > position, FwdList&& other);
    void splice_after(Iterator< T > position, FwdList& other, Iterator< T > i);
    void splice_after(Iterator< T > position, FwdList&& other, Iterator< T > i);
    void splice_after(Iterator< T > position, FwdList& other, Iterator< T > first, Iterator< T > last);
    void splice_after(Iterator< T > position, FwdList&& other, Iterator< T > first, Iterator< T > last);

    private:
      Node< T >* fake_;
      size_t size_;
  };

  template< typename T >
  FwdList< T >::FwdList():
    fake_(new Node< T >(T())),
    size_(0)
  {
    fake_ -> next_ = fake_;
  }

  template< typename T >
  FwdList< T >::FwdList(size_t count, const T& value):
    FwdList()
  {
    while (count--)
    {
      push_back(value);
    }
  }

  template< typename T >
  FwdList< T >::FwdList(std::initializer_list< T > init):
    FwdList()
  {
    for (const T& value: init)
    {
      push_back(value);
    }
  }

  template< typename T >
  FwdList< T >::FwdList(const FwdList& other):
    FwdList()
  {
    for (Iterator< T > it = other.cbegin(); it != other.cend(); ++it)
    {
      push_back(*it);
    }
  }

  template< typename T >
  FwdList< T >::FwdList(FwdList&& other) noexcept:
    fake_(other.fake_),
    size_(other.size_)
  {
    other.fake_ = new Node< T >();
    other.fake_ -> next_ = other.fake_;
    other.size_ = 0;
  }

  template< typename T >
  FwdList< T >::~FwdList()
  {
    clear();
    delete fake_;
  }

  template< typename T >
  typename FwdList< T >::FwdList& FwdList< T >::operator=(const FwdList& other) noexcept
  {
    if (size() != other.size())
    {
      return false;
    }
    auto it1 = cbegin();
    auto it2 = other.cbegin();
    while (it1 != cend())
    {
      if (*it1 != *it2)
      {
        return false;
      }
      ++it1;
      ++it2;
    }
    return true;
  }

  template< typename T >
  typename FwdList< T >::FwdList& FwdList< T >::operator=(FwdList&& other) noexcept
  {
    return !(*this == other);
  }

  template< typename T >
  Iterator< T > FwdList< T >::begin() const noexcept
  {
    return Iterator< T >(fake_ -> next_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::end() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::cbegin() const noexcept
  {
    return Iterator< T >(fake_ -> next_);
  }

  template< typename T >
  Iterator< T > FwdList< T >::cend() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  Iterator< T > FwdList<T>::before_begin() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  Iterator< T > FwdList<T>::cbefore_begin() const noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  T& FwdList< T >::front() const noexcept
  {
    assert(!empty());
    return fake_ -> next_ -> data_;
  }

  template< typename T >
  T& FwdList< T >::back() const noexcept
  {
    assert(!empty());
    Node< T >* current = fake_;
    while (current -> next_ != fake_)
    {
      current = current -> next_;
    }
    return current -> data_;
  }

  template< typename T >
  size_t FwdList< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool FwdList< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  void FwdList< T >::assign(size_t size, const T& value)
  {
    for (size_t i = 0; i < size; ++i)
    {
      push_front(value);
    }
  }

  template< typename T >
  void FwdList< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    newNode -> next_ = fake_ -> next_;
    fake_ -> next_ = newNode;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    Node< T >* current = fake_;
    while (current -> next_ != fake_)
    {
      current = current -> next_;
    }
    current -> next_ = newNode;
    newNode -> next_ = fake_;
    ++size_;
  }

  template< typename T >
  void FwdList< T >::pop_front() noexcept
  {
    assert(!empty());
    Node< T >* todelete = fake_ -> next_;
    fake_ -> next_ = todelete -> next_;
    delete todelete;
    --size_;
  }

  template < typename T >
  void FwdList< T >::pop_back() noexcept
  {
    assert(!empty());
    Node< T >* todelete = fake_;
    while (todelete -> next_ -> next_ != fake_)
    {
      todelete = todelete -> next_;
    }
    delete todelete -> next_;
    todelete -> next_ = fake_;
    --size_;
  }

  template< typename T >
  void FwdList< T >::swap(FwdList& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  void FwdList< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  void FwdList< T >::remove(const T& value)
  {
    Node< T >* current = fake_;
    while (current -> next_ != fake_)
    {
      if (current -> next_ -> data_ == value)
      {
        Node< T >* todelete = current -> next_;
        current -> next_ = current -> next_ -> next_;
        delete todelete;
        --size_;
      }
      else
      {
        current = current -> next_;
      }
    }
  }

  template< typename T >
  template< typename Predicate >
  void FwdList< T >::remove_if(Predicate predicate)
  {
    Node< T >* current = fake_;
    while (current -> next_ != fake_)
    {
      if (predicate(current -> next_ -> data_))
      {
        Node< T >* todelete = current -> next_;
        current -> next_ = current -> next_ -> next_;
        delete todelete;
        --size_;
      }
      else
      {
        current = current -> next_;
      }
    }
  }
}

#endif
