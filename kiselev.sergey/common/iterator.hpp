#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <iterator>
#include <cassert>
#include "treeNode.hpp"

namespace kiselev
{
  template< typename Key, typename Value, typename Cmp >
  class RBTree;

  namespace detail
  {
    template< typename Key, typename Value, typename Cmp, bool IsConst >
    class Iterator: public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
    {
    public:
      using value = std::pair< Key, Value >;
      using reference = std::conditional_t< IsConst, const value&, value& >;
      using pointer = std::conditional_t< IsConst, const value*, value* >;
      using iterator = Iterator< Key, Value, Cmp, IsConst >;

      Iterator();
      template < bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      Iterator(const Iterator< Key, Value, Cmp, OtherIsConst >&) noexcept;
      template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > = 0 >
      iterator& operator=(const Iterator< Key, Value, Cmp, OtherIsConst >&) noexcept;

      iterator& operator++() noexcept;
      iterator operator++(int) noexcept;

      iterator& operator--() noexcept;
      iterator operator--(int) noexcept;

      reference operator*() const noexcept;
      pointer operator->() const noexcept;

      bool operator==(const iterator&) const noexcept;
      bool operator!=(const iterator&) const noexcept;
    private:
      TreeNode< Key, Value >* node_;
      TreeNode< Key, Value >* parent_;
      Iterator(TreeNode< Key, Value >*, TreeNode< Key, Value >*);
      friend class Iterator< Key, Value, Cmp, !IsConst >;
      friend class RBTree< Key, Value, Cmp >;
    };

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >::Iterator():
      node_(nullptr),
      parent_(nullptr)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    Iterator< Key, Value, Cmp, IsConst >::Iterator(const Iterator< Key, Value, Cmp, OtherIsConst >& other) noexcept:
      node_(other.node_),
      parent_(other.parent_)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >::Iterator(TreeNode< Key, Value >* node, TreeNode< Key, Value >* parent):
      node_(node),
      parent_(parent)
    {}

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    template< bool OtherIsConst, std::enable_if_t< IsConst && !OtherIsConst, int > >
    Iterator< Key, Value, Cmp, IsConst >& Iterator< Key, Value, Cmp, IsConst >::operator=(
      const Iterator< Key, Value, Cmp, OtherIsConst >& other) noexcept
    {
      node_ = other.node_;
      parent_ = other.parent_;
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >& Iterator< Key, Value, Cmp, IsConst >::operator++() noexcept
    {
      assert(node_ != nullptr);
      if (node_->right)
      {
        node_ = node_->right;
        while (node_->left)
        {
          node_ = node_->left;
        }
      }
      else
      {
        while (node_->parent && node_ == node_->parent->right)
        {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
      if (node_)
      {
        parent_ = node_->parent;
      }
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst > Iterator< Key, Value, Cmp, IsConst >::operator++(int) noexcept
    {
      assert(node_ != nullptr);
      iterator result(*this);
      ++(*this);
      return result;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst >& Iterator< Key, Value, Cmp, IsConst >::operator--() noexcept
    {
      if (!node_ && !parent_)
      {
        node_ = parent_;
        parent_ = node_->parent;
      }
      else
      {
        if (node_->left)
        {
          node_ = node_->left;
          while (node_->right)
          {
            node_ = node_->right;
          }
          return iterator(node_);
        }
        while (node_->parent && node_ == node_->parent->left)
        {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
      return *this;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    Iterator< Key, Value, Cmp, IsConst > Iterator< Key, Value, Cmp, IsConst >::operator--(int) noexcept
    {
      iterator result(*this);
      --(*this);
      return result;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename Iterator< Key, Value, Cmp, IsConst >::reference Iterator< Key, Value, Cmp, IsConst >::operator*() const noexcept
    {
      assert(node_ != nullptr);
      return node_->data;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    typename Iterator< Key, Value, Cmp, IsConst >::pointer Iterator< Key, Value, Cmp, IsConst >::operator->() const noexcept
    {
      assert(node_ != nullptr);
      return std::addressof(node_->data);
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool Iterator< Key, Value, Cmp, IsConst >::operator==(const iterator& other) const noexcept
    {
      return node_ == other.node_;
    }

    template< typename Key, typename Value, typename Cmp, bool IsConst >
    bool Iterator< Key, Value, Cmp, IsConst >::operator!=(const iterator& other) const noexcept
    {
      return !(*this == other);
    }
  }
}
#endif
