#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <iterator>
#include <cassert>
#include "node.hpp"

namespace averenkov
{
  template< class T >
  struct ConstListIterator : public std::iterator<std::forward_iterator_tag, T>
  {
  public:
    ConstListIterator() = default;
    ConstListIterator(Node< T >* node) : node_(node) {};
    ~ConstListIterator() = default;

    ConstListIterator< T >& operator++();
    ConstListIterator< T > operator++(int);

    const T& operator*();
    const T* operator->();

    bool operator==(const ConstListIterator< T >&) const;
    bool operator!=(const ConstListIterator< T >&) const;

    Node< T >* getNode();

  private:
    Node< T >* node_;

  };

  template< class T >
  Node< T >*  ConstListIterator< T >::getNode()
  {
    return node_;
  }

  template< class T >
  ConstListIterator< T >& ConstListIterator< T >::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->next_;
    return *this;
  }

  template< class T >
  ConstListIterator< T > ConstListIterator< T >::operator++(int)
  {
    assert(node_ != nullptr);
    ConstListIterator< T > result(*this);
    ++(*this);
    return result;
  }

  template< class T >
  const T& ConstListIterator< T >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< class T >
  const T* ConstListIterator< T >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< class T >
  bool ConstListIterator< T >::operator==(const ConstListIterator< T >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool ConstListIterator< T >::operator!=(const ConstListIterator< T >& rhs) const
  {
    return !(rhs == *this);
  }
}

#endif
