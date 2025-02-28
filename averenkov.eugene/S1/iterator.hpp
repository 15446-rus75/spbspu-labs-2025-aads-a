#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <cassert>
#include "node.hpp"

namespace averenkov
{
  template< class T >
  struct ListIterator : public std::iterator<std::forward_iterator_tag, T>
  {
  public:
    ListIterator() = default;
    ListIterator(Node<T>* node) : node_(node) {};
    ~ListIterator() = default;

    ListIterator< T >& operator++();
    ListIterator< T > operator++(int);

    T& operator*();
    T* operator->();

    bool operator==(const ListIterator< T >&) const;
    bool operator!=(const ListIterator< T >&) const;

  private:
    Node<T>* node_;

  };


  template< typename T >
  ListIterator< T >& ListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< typename T >
  ListIterator< T > ListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    ListIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  T& ListIterator< T >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  T* ListIterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool ListIterator< T >::operator==(const ListIterator< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  bool ListIterator< T >::operator!=(const ListIterator< T >& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
