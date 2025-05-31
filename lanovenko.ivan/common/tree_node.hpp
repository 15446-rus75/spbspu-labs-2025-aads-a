#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <utility>

namespace lanovenko
{
  template< typename Key, typename Value >
  struct TreeNode
  {
    std::pair< Key, Value > data_;
    unsigned short int height_;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode* parent_;
    TreeNode() = delete;
    TreeNode(const std::pair< Key, Value >& data);
  };
}

template< typename Key, typename Value >
lanovenko::TreeNode< Key, Value >::TreeNode(const std::pair< Key, Value >& data):
  data_(data),
  height_(1),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}


#endif
