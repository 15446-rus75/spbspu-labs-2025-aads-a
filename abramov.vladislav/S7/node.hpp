#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace abramov
{
  template< class Key, class Value >
  struct Node
  {
    std::pair< Key, Value > data_;
    Node< Key, Value > *next_;
    bool active_;

    Node(const Key &k, const Value &v);
  };
}

template< class Key, class Value >
abramov::Node< Key, Value >::Node(const Key &k, const Value &v):
  data_(std::make_pair(k, v)),
  next_(nullptr),
  active_(true)
{}
#endif
