#ifndef TREE_HPP
#define TREE_HPP
#include <functional>
#include "node.hpp"
#include "iterator.hpp"
#include "cIterator.hpp"

namespace demehin
{
  template< typename Key, typename T, typename Cmp = std::less< Key > >
  class Tree
  {
  public:
    using cIter = TreeIterator< Key, T, Cmp >;
    using Iter = TreeIterator< Key, T, Cmp >;
    using DataPair = std::pair< Key, T >;

    Tree();
    Tree(const Tree< Key, T, Cmp >&);
    Tree(Tree< Key, T, Cmp >&&);

    ~Tree();

    Tree< Key, T, Cmp >& operator=(const Tree< Key, T, Cmp >&);
    std::pair< Iter, bool > insert(const DataPair&);

    Iter erase(Iter) noexcept;
    size_t erase(const Key&) noexcept;
    Iter erase(Iter, Iter) noexcept;

    T& at(const Key&);
    const T& at(const Key&) const;
    T& operator[](const Key&);

    Iter find(const Key&) const noexcept;

    Iter begin() const noexcept;
    cIter cbegin() const noexcept;
    Iter end() const noexcept;
    cIter cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

    size_t count(const Key&) const noexcept;

    void swap(Tree< Key, T, Cmp >&) noexcept;

  private:
    using Node = demehin::TreeNode< Key, T >;

    Node* fakeRoot_;
    Node* root_;
    Cmp cmp_;
    size_t size_;

    int height(Node*) const noexcept;
    Node* rotateRight(Node*) noexcept;
    Node* rotateLeft(Node*) noexcept;
    Node* balance(Node*) noexcept;
    void balanceUpper(Node*) noexcept;
    int getBalanceFactor(Node*) const noexcept;
    void updateHeight(Node*) noexcept;
  };

  template< typename Key, typename T, typename Cmp >
  Tree< Key, T, Cmp >::Tree():
    fakeRoot_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    root_(fakeRoot_),
    cmp_(),
    size_(0)
  {
    fakeRoot_->left = fakeRoot_->right = fakeRoot_;
    fakeRoot_->height = -1;
    fakeRoot_->parent = nullptr;
  }

  template< typename Key, typename T, typename Cmp >
  Tree< Key, T, Cmp >::Tree(const Tree< Key, T, Cmp >& other):
    fakeRoot_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    root_(fakeRoot_),
    cmp_(other.cmp_),
    size_(0)
  {
    fakeRoot_->left = fakeRoot_->right = fakeRoot_;
    fakeRoot_->height = -1;
    for (auto it = other.begin(); it != other.end(); it++)
    {
      insert(*it);
    }
  }

  template< typename Key, typename T, typename Cmp >
  Tree< Key, T, Cmp >::Tree(Tree< Key, T, Cmp >&& other):
    fakeRoot_(std::exchange(other.fakeRoot_, nullptr)),
    root_(std::exchange(other.root_, nullptr)),
    cmp_(std::move(other.cmp_)),
    size_(std::exchange(other.size_, 0))
  {}

  template< typename Key, typename T, typename Cmp >
  Tree< Key, T, Cmp >& Tree< Key, T, Cmp >::operator=(const Tree< Key, T, Cmp >& rhs)
  {
    if (this != &rhs)
    {
      Tree< Key, T, Cmp > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename T, typename Cmp >
  Tree< Key, T, Cmp >::~Tree()
  {
    if (fakeRoot_ != nullptr)
    {
      clear();
      delete[] reinterpret_cast< char* >(fakeRoot_);
    }
  }

  template< typename Key, typename T, typename Cmp >
  void Tree< Key, T, Cmp >::clear() noexcept
  {
    erase(begin(), end());
    fakeRoot_->left = fakeRoot_->right = fakeRoot_;
    root_ = fakeRoot_;
    size_ = 0;
  }

  template< typename Key, typename T, typename Cmp >
  std::pair< TreeIterator< Key, T, Cmp >, bool > Tree< Key, T, Cmp >::insert(const DataPair& value)
  {
    Node* newNode = new Node(value);
    if (root_ == fakeRoot_)
    {
      root_ = newNode;
      root_->parent = fakeRoot_;
      fakeRoot_->left = fakeRoot_->right = root_;
      size_++;
      return { Iter(root_), true };
    }
    Node* current = root_;
    Node* parent = fakeRoot_;
    bool isLeft = false;
    while (current != fakeRoot_ && current != nullptr)
    {
      parent = current;
      if (cmp_(value.first, current->data.first))
      {
        current = current->left;
        isLeft = true;
      }
      else if (cmp_(current->data.first, value.first))
      {
        current = current->right;
        isLeft = false;
      }
      else
      {
        delete newNode;
        return { Iter(current), false };
      }

    }

    newNode->parent = parent;
    if (isLeft)
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }

    newNode->left = newNode->right = nullptr;
    balanceUpper(newNode);
    size_++;
    return { Iter(newNode), true };
  }

  template< typename Key, typename T, typename Cmp >
  int Tree< Key, T, Cmp >::height(Node* node) const noexcept
  {
    return (node == fakeRoot_ || node == nullptr) ? -1 : node->height;
  }

  template< typename Key, typename T, typename Cmp >
  void Tree< Key, T, Cmp >::updateHeight(Node* node) noexcept
  {
    if (node != fakeRoot_ && node != nullptr)
    {
      node->height = 1 + std::max(height(node->left), height(node->right));
    }
  }

  template< typename Key, typename T, typename Cmp >
  int Tree< Key, T, Cmp >::getBalanceFactor(Node* node) const noexcept
  {
    if (node == fakeRoot_ || node == nullptr)
    {
      return 0;
    }
    return height(node->left) - height(node->right);
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Node* Tree< Key, T, Cmp >::rotateRight(Node* node) noexcept
  {
    Node* lt = node->left;
    node->left = lt->right;
    if (lt->right != nullptr)
    {
      lt->right->parent = node;
    }

    lt->parent = node->parent;
    node->parent = lt;
    lt->right = node;
    if (lt->parent != fakeRoot_)
    {
      if (lt->parent->left == node)
      {
        lt->parent->left = lt;
      }
      else
      {
        lt->parent->right = lt;
      }
    }
    else
    {
      root_ = lt;
      fakeRoot_->right = lt;
    }

    updateHeight(node);
    updateHeight(lt);

    return lt;
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Node* Tree< Key, T, Cmp >::rotateLeft(Node* node) noexcept
  {
    Node* rt = node->right;
    node->right = rt->left;
    if (rt->left != nullptr)
    {
      rt->left->parent = node;
    }

    rt->parent = node->parent;
    node->parent = rt;
    rt->left = node;

    if (rt->parent != fakeRoot_)
    {
      if (rt->parent->left == node)
      {
        rt->parent->left = rt;
      }
      else
      {
        rt->parent->right = rt;
      }
    }
    else
    {
      root_ = rt;
      fakeRoot_->right = rt;
    }

    updateHeight(node);
    updateHeight(rt);

    return rt;
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Node* Tree< Key, T, Cmp >::balance(Node* node) noexcept
  {
    updateHeight(node);
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1)
    {
      if (getBalanceFactor(node->left) < 0)
      {
        node->left = rotateLeft(node->left);
      }
      return rotateRight(node);
    }

    if (balanceFactor < -1)
    {
      if (getBalanceFactor(node->right) > 0)
      {
        node->right = rotateRight(node->right);
      }
      return rotateLeft(node);
    }

    return node;
  }

  template< typename Key, typename T, typename Cmp >
  void Tree< Key, T, Cmp >::balanceUpper(Node* node) noexcept
  {
    while (node != fakeRoot_)
    {
      Node* parent = node->parent;
      bool isLeft = (parent->left == node);
      node = balance(node);
      if (isLeft)
      {
        parent->left = node;
      }
      else
      {
        parent->right = node;
      }
      node = parent;
    }
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Iter Tree< Key, T, Cmp >::erase(Iter pos) noexcept
  {
    if (pos == end())
    {
      return end();
    }

    Node* todelete = pos.getNode();
    Node* parent = todelete->parent;
    Iter result = Iter(parent);

    if (todelete->left == nullptr && todelete->right == nullptr)
    {
      if (parent->left == todelete)
      {
        parent->left = nullptr;
      }
      else
      {
        parent->right = nullptr;
      }
      result = Iter(parent);
    }
    else if (todelete->left == nullptr || todelete->right == nullptr)
    {
      Node* child = (todelete->left != nullptr) ? todelete->left : todelete->right;
      if (parent->left == todelete)
      {
        parent->left = child;
      }
      else
      {
        parent->right = child;
      }
      child->parent = parent;
      result = Iter(child);
    }
    else
    {
      Node* next = todelete->right;
      while (next->left != nullptr)
      {
        next = next->left;
      }
      todelete->data = next->data;
      return erase(Iter(next));
    }

    if (todelete == root_)
    {
      root_ = (parent != fakeRoot_) ? parent : fakeRoot_;
      result = begin();
    }

    delete todelete;
    size_--;
    balanceUpper(parent);
    return result;
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Iter Tree< Key, T, Cmp >::erase(Iter first, Iter last) noexcept
  {
    for (auto it = first; it != last;)
    {
      it = erase(it);
    }
    return last;
  }

  template< typename Key, typename T, typename Cmp >
  size_t Tree< Key, T, Cmp >::erase(const Key& key) noexcept
  {
    Iter it = find(key);
    if (it == end())
    {
      return 0;
    }

    erase(it);
    return 1;
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Iter Tree< Key, T, Cmp >::begin() const noexcept
  {
    if (empty())
    {
      return end();
    }
    Node* current = root_;
    while (current->left != nullptr)
    {
      current = current->left;
    }
    return Iter(current);
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::cIter Tree< Key, T, Cmp >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    Node* current = root_;
    while (current->left != nullptr)
    {
      current = current->left;
    }
    return cIter(current);
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Iter Tree< Key, T, Cmp >::end() const noexcept
  {
    return Iter(fakeRoot_);
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::cIter Tree< Key, T, Cmp >::cend() const noexcept
  {
    return cIter(fakeRoot_);
  }

  template< typename Key, typename T, typename Cmp >
  T& Tree< Key, T, Cmp >::at(const Key& key)
  {
    auto searched = find(key);
    if (searched == end())
    {
      throw std::out_of_range("key not found");
    }
    return searched->second;
  }

  template< typename Key, typename T, typename Cmp >
  const T& Tree< Key, T, Cmp >::at(const Key& key) const
  {
    auto searched = find(key);
    if (searched == end())
    {
      throw std::out_of_range("key not found");
    }
    return searched->second;
  }

  template< typename Key, typename T, typename Cmp >
  T& Tree< Key, T, Cmp >::operator[](const Key& key)
  {
    auto searched = find(key);
    if (searched == end())
    {
      insert(std::make_pair(key, T()));
      searched = find(key);
    }
    return searched->second;
  }

  template< typename Key, typename T, typename Cmp >
  typename Tree< Key, T, Cmp >::Iter Tree< Key, T, Cmp >::find(const Key& key) const noexcept
  {
    Node* current = root_;
    while (current != nullptr && current != fakeRoot_)
    {
      if (cmp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (cmp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        return Iter(current);
      }
    }
    return end();
  }

  template< typename Key, typename T, typename Cmp >
  size_t Tree< Key, T, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename T, typename Cmp >
  bool Tree< Key, T, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename T, typename Cmp >
  void Tree< Key, T, Cmp >::swap(Tree< Key, T, Cmp >& rhs) noexcept
  {
    std::swap(fakeRoot_, rhs.fakeRoot_);
    std::swap(root_, rhs.root_);
    std::swap(cmp_, rhs.cmp_);
    std::swap(size_, rhs.size_);
  }

  template< typename Key, typename T, typename Cmp >
  size_t Tree< Key, T, Cmp >::count(const Key& key) const noexcept
  {
    return (find(key) != end());
  }

}

#endif
