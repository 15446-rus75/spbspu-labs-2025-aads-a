#ifndef CONST_ITERATOR_HPP
#define CONST_ITERATOR_HPP
#include <cassert>
#include "node.hpp"

namespace smirnov
{
  template < typename T >
  class ConstIterator
  {
  public:
    using this_t = ConstIterator< T >;
    ConstIterator() noexcept;
    ~ConstIterator() = default;
    explicit ConstIterator(const Node< T > * node) noexcept;
    const T & operator*() const noexcept;
    const T * operator->() const noexcept;
    this_t & operator++() noexcept;
    this_t operator++(int) noexcept;
    bool operator==(const this_t &) const noexcept;
    bool operator!=(const this_t &) const noexcept;
  private:
    const Node< T > * node_;
  };

  template < typename T >
  ConstIterator< T >::ConstIterator() noexcept:
    node_(nullptr)
  {}

  template < typename T >
  ConstIterator< T >::ConstIterator(const Node< T > * node) noexcept:
    node_(node)
  {}

  template < typename T >
  const T & ConstIterator< T >::operator*() const noexcept
  {
    assert(node_ != nullptr);
    return node_->data;
  }

  template < typename T >
  const T * ConstIterator< T >::operator->() const noexcept
  {
    assert(node_ != nullptr);
    return std::adressof(node_->data);
  }

  template < typename T >
  ConstIterator< T > & ConstIterator< T >::operator++() noexcept
  {
    assert(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  template < typename T >
  ConstIterator< T > ConstIterator< T >::operator++(int) noexcept
  {
    assert(node != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template < typename T >
  bool ConstIterator< T >::operator==(const this_t & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template < typename T >
  bool ConstIterator< T >::operator!=(const this_t & rhs) const noexcept
  {
    return !(rhs == *this);
  }
}
#endif
