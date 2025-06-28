#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include "node.hpp"

namespace abramov
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value, class Hash, class Equal >
  struct Iterator: std::iterator< std::forward_iterator_tag, std::pair< Key, Value > >
  {
    using HashT = HashTable< Key, Value, Hash, Equal >;
    using Iter = const Iterator< Key, Value, Hash, Equal >;

    Iterator();
    Iterator(HashTable< Key, Value, Hash, Equal > *t, size_t i, Node< Key, Value > *n);
    Iterator(const Iterator< Key, Value, Hash, Equal > &iter) = default;
    ~Iterator() = default;
    Iterator< Key, Value, Hash, Equal > &operator=(const Iterator< Key, Value, Hash, Equal > &iter) = default;
    Iterator< Key, Value, Hash, Equal > &operator++() noexcept;
    Iterator< Key, Value, Hash, Equal > operator++(int) noexcept;
    bool operator==(const Iterator< Key, Value, Hash, Equal > &iter) const noexcept;
    bool operator!=(const Iterator< Key, Value, Hash, Equal > &iter) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;
    std::pair< Key, Value > *operator->() noexcept;
  private:
    HashTable< Key, Value, Hash, Equal > *table_;
    size_t ind_;
    Node< Key, Value > *node_;

    void goNext() noexcept;
  };
}

template< class Key, class Value, class Hash, class Equal >
abramov::Iterator< Key, Value, Hash, Equal >::Iterator():
  table_(nullptr),
  ind_(0),
  node_(nullptr)
{}

template< class Key, class Value, class Hash, class Equal >
abramov::Iterator< Key, Value, Hash, Equal >::Iterator(HashT *t, size_t i, Node< Key, Value > *n):
  table_(t),
  ind_(i),
  node_(n)
{}

template< class Key, class Value, class Hash, class Equal >
abramov::Iterator< Key, Value, Hash, Equal > &abramov::Iterator< Key, Value, Hash, Equal >::operator++() noexcept
{
  if (node_)
  {
    node_ = node_->next_;
    if (!node_)
    {
      goNext();
    }
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void abramov::Iterator< Key, Value, Hash, Equal >::goNext() noexcept
{
  ++ind_;
  while (ind_ < table_->capacity_ && !table_->table_[ind_])
  {
    ++ind_;
  }
  if (ind_ < table_->capacity_)
  {
    node_ = table_->table_[ind_];
  }
  else
  {
    node_ = nullptr;
  }
}

template< class Key, class Value, class Hash, class Equal >
abramov::Iterator< Key, Value, Hash, Equal > abramov::Iterator< Key, Value, Hash, Equal >::operator++(int) noexcept
{
  Iterator< Key, Value, Hash, Equal > tmp(*this);
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool abramov::Iterator< Key, Value, Hash, Equal >::operator==(Iter &rhs) const noexcept
{
  return node_ == rhs.node_;
}

template< class Key, class Value, class Hash, class Equal >
bool abramov::Iterator< Key, Value, Hash, Equal >::operator!=(Iter &rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &abramov::Iterator< Key, Value, Hash, Equal >::operator*() noexcept
{
  return node_->data_;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > *abramov::Iterator< Key, Value, Hash, Equal >::operator->() noexcept
{
  return std::addressof(node_->data_);
}
#endif
