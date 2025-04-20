#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <utility>
#include "iterator.hpp"
#include "cIterator.hpp"

namespace duhanina
{
  template < typename Key, typename Value, typename Compare >
  class Tree
  {
  public:
    using Node_t = Node< Key, Value >;
    using Iterator_t = Iterator< Key, Value, Compare >;
    using ConstIterator_t = ConstIterator< Key, Value, Compare >;

    Tree();
    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    ~Tree();

    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    Iterator_t begin() const noexcept;
    ConstIterator_t cbegin() const noexcept;
    Iterator_t end() const noexcept;
    ConstIterator_t cend() const noexcept;

    void push(const Key& k, const Value& v);
    Value get(const Key& k) const;
    Value drop(const Key& k);

    size_t size() const;
    bool empty() const;

    void clear();
    void swap(Tree& other);

    Iterator_t insert(const std::pair< const Key, Value >& value);
    Iterator_t insert(std::pair< const Key, Value >&& value);
    template< typename InputIt >
    void insert(InputIt first, InputIt last);

    Iterator_t erase(Iterator_t it) noexcept;
    size_t erase(const Key& key) noexcept;
    Iterator_t erase(Iterator_t first, Iterator_t last) noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    size_t count(const Key& key) const;
    Iterator_t find(const Key& key) noexcept;
    ConstIterator_t find(const Key& key) const noexcept;

    std::pair< Node_t*, Node_t* > equal_range(const Key& key);
    std::pair< const Node_t*, const Node_t* > equal_range(const Key& key) const;
    Iterator_t lower_bound(const Key& key);
    ConstIterator_t lower_bound(const Key& key) const;
    Iterator_t upper_bound(const Key& key);
    ConstIterator_t upper_bound(const Key& key) const;

  private:
    Node_t* fakeRoot_;
    size_t size_;
    Compare comp_;

    Node_t* getRoot() const;
    void setRoot(Node_t* newRoot);
    void updateHeight(Node_t* Node_t);
    int getBalance(Node_t* node);
    Node_t* rotateRight(Node_t* y);
    Node_t* rotateLeft(Node_t* x);
    Node_t* insert(Node_t* node, const Key& k, const Value& v, Node_t* parent);
    Node_t* find(Node_t* node, const Key& k) const;
    Node_t* findMin(Node_t* node) const;
    Node_t* findMax(Node_t* node) const;
    Node_t* remove(Node_t* node, const Key& k);
    void clear(Node_t* node);
    Node_t* copyTree(Node_t* node, Node_t* parent);
    Node_t* findNextNode(Node_t* node) const;
  };

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree():
    fakeRoot_(new Node_t(Key(), Value(), nullptr)),
    size_(0)
  {}

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree(const Tree& other):
    fakeRoot_(new Node_t(Key(), Value(), nullptr)),
    size_(other.size_)
  {
    setRoot(copyTree(other.getRoot(), fakeRoot_));
  }

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::Tree(Tree&& other) noexcept:
    fakeRoot_(other.fakeRoot_),
    size_(other.size_)
  {
    other.fakeRoot_ = new Node_t(Key(), Value(), nullptr);
    other.size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >::~Tree< Key, Value, Compare >()
  {
    clear();
    delete fakeRoot_;
  }

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(const Tree< Key, Value, Compare >& other)
  {
    if (this != std::addressof(other))
    {
      clear();
      setRoot(copyTree(other.getRoot(), fakeRoot_));
      size_ = other.size_;
    }
    return *this;
  }

  template < typename Key, typename Value, typename Compare >
  Tree< Key, Value, Compare >& Tree< Key, Value, Compare >::operator=(Tree< Key, Value, Compare >&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      clear();
      delete fakeRoot_;
      fakeRoot_ = other.fakeRoot_;
      size_ = other.size_;
      other.fakeRoot_ = new Node_t(Key(), Value(), nullptr);
      other.size_ = 0;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::begin() const noexcept
  {
    if (empty())
    {
      return end();
    }
    return Iterator_t(findMin(getRoot()));
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    return ConstIterator_t(findMin(getRoot()));
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::end() const noexcept
  {
    return Iterator_t(fakeRoot_);
  }

  template< typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::cend() const noexcept
  {
    return ConstIterator_t(fakeRoot_);
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::push(const Key& k, const Value& v)
  {
    setRoot(insert(getRoot(), k, v, fakeRoot_));
    size_++;
  }

  template < typename Key, typename Value, typename Compare >
  Value Tree< Key, Value, Compare >::get(const Key& k) const
  {
    Node_t* node = find(getRoot(), k);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  Value Tree< Key, Value, Compare >::drop(const Key& k)
  {
    Value value = get(k);
    setRoot(remove(getRoot(), k));
    size_--;
    return value;
  }

  template < typename Key, typename Value, typename Compare >
  size_t Tree< Key, Value, Compare >::size() const
  {
    return size_;
  }

  template < typename Key, typename Value, typename Compare >
  bool Tree< Key, Value, Compare >::empty() const
  {
    return size_ == 0;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::clear()
  {
    clear(getRoot());
    setRoot(nullptr);
    size_ = 0;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::swap(Tree< Key, Value, Compare >& other)
  {
    std::swap(fakeRoot_, other.fakeRoot_);
    std::swap(size_, other.size_);
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::insert(const std::pair< const Key, Value >& value)
  {
    push(value.first, value.second);
    return Iterator_t(find(getRoot(), value.first));
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::insert(std::pair< const Key, Value >&& value)
  {
    push(std::move(value.first), std::move(value.second));
    return Iterator_t(find(getRoot(), value.first));
  }

  template < typename Key, typename Value, typename Compare >
  template< typename InputIt >
  void Tree< Key, Value, Compare >::insert(InputIt first, InputIt last)
  {
    for (auto it = first; it != last; ++it)
    {
      push(it->first, it->second);
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::erase(Iterator_t pos) noexcept
  {
    if (pos == end() || pos.current_ == nullptr)
    {
      return end();
    }
    Node_t* nextNode = findNextNode(pos.current_);
    setRoot(remove(getRoot(), pos.current_->data.first));
    size_--;
    if (nextNode != nullptr)
    {
      return Iterator_t(nextNode);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  size_t Tree< Key, Value, Compare >::erase(const Key& key) noexcept
  {
    Iterator_t it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::erase(Iterator_t first, Iterator_t last) noexcept
  {
    while (first != last)
    {
      first = erase(first);
    }
    return last;
  }

  template < typename Key, typename Value, typename Compare >
  Value& Tree< Key, Value, Compare >::at(const Key& key)
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  const Value& Tree< Key, Value, Compare >::at(const Key& key) const
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  Value& Tree< Key, Value, Compare >::operator[](const Key& key)
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      push(key, Value());
      node = find(getRoot(), key);
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  Value& Tree< Key, Value, Compare >::operator[](Key&& key)
  {
    Node_t* node = find(getRoot(), key);
    if (!node)
    {
      push(std::move(key), Value());
      node = find(getRoot(), key);
    }
    return node->data.second;
  }

  template < typename Key, typename Value, typename Compare >
  size_t Tree< Key, Value, Compare >::count(const Key& key) const
  {
    if (find(getRoot(), key))
    {
      return 1;
    }
    return 0;
  }


  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::find(const Key& key) noexcept
  {
    Node_t* node = find(getRoot(), key);
    if (node != nullptr)
    {
      return Iterator_t(node);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::find(const Key& key) const noexcept
  {
    const Node_t* node = find(getRoot(), key);
    if (node != nullptr)
    {
      return ConstIterator_t(node);
    }
    else
    {
      return cend();
    }
  }

  template < typename Key, typename Value, typename Compare >
  std::pair< typename Tree< Key, Value, Compare >::Node_t*, typename Tree< Key, Value, Compare >::Node_t* >
    Tree< Key, Value, Compare >::equal_range(const Key& key)
  {
    return { lower_bound(key), upper_bound(key) };
  }

  template < typename Key, typename Value, typename Compare >
  std::pair< const typename Tree< Key, Value, Compare >::Node_t*, const typename Tree< Key, Value, Compare >::Node_t* >
    Tree< Key, Value, Compare >::equal_range(const Key& key) const
  {
    return { lower_bound(key), upper_bound(key) };
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::lower_bound(const Key& key)
  {
    Node_t* current = getRoot();
    Node_t* result = nullptr;
    while (current)
    {
      if (!comp_(current->data.first, key))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return Iterator_t(result);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::lower_bound(const Key& key) const
  {
    const Node_t* current = getRoot();
    const Node_t* result = nullptr;
    while (current)
    {
      if (!comp_(current->data.first, key))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return ConstIterator_t(result);
    }
    else
    {
      return cend();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Iterator_t Tree< Key, Value, Compare >::upper_bound(const Key& key)
  {
    Node_t* current = getRoot();
    Node_t* result = nullptr;
    while (current)
    {
      if (comp_(key, current->data.first))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return Iterator_t(result);
    }
    else
    {
      return end();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::ConstIterator_t Tree< Key, Value, Compare >::upper_bound(const Key& key) const
  {
    const Node_t* current = getRoot();
    const Node_t* result = nullptr;
    while (current)
    {
      if (comp_(key, current->data.first))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    if (result != nullptr)
    {
      return ConstIterator_t(result);
    }
    else
    {
      return cend();
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::getRoot() const
  {
    return fakeRoot_->left;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::setRoot(Node_t* newRoot)
  {
    fakeRoot_->left = newRoot;
    if (newRoot)
    {
      newRoot->parent = fakeRoot_;
    }
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::updateHeight(Node_t* node)
  {
    if (node == nullptr)
    {
      return;
    }
    int leftHeight = 0;
    if (node->left != nullptr)
    {
      leftHeight = node->left->height;
    }
    int rightHeight = 0;
    if (node->right != nullptr)
    {
      rightHeight = node->right->height;
    }
    node->height = 1 + std::max(leftHeight, rightHeight);
  }

  template < typename Key, typename Value, typename Compare >
  int Tree< Key, Value, Compare >::getBalance(Node_t* node)
  {
    if (!node)
    {
      return 0;
    }
    int leftHeight = 0;
    if (node->left)
    {
      leftHeight = node->left->height;
    }
    int rightHeight = 0;
    if (node->right)
    {
      rightHeight = node->right->height;
    }
    return leftHeight - rightHeight;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::rotateRight(Node_t* root)
  {
    Node_t* leftChild = root->left;
    Node_t* leftRightSubtree = leftChild->right;
    leftChild->right = root;
    root->left = leftRightSubtree;
    if (leftRightSubtree)
    {
      leftRightSubtree->parent = root;
    }
    leftChild->parent = root->parent;
    root->parent = leftChild;
    updateHeight(root);
    updateHeight(leftChild);
    return leftChild;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::rotateLeft(Node_t* root)
  {
    Node_t* rightChild = root->right;
    Node_t* rightLeftSubtree = rightChild->left;
    rightChild->left = root;
    root->right = rightLeftSubtree;
    if (rightLeftSubtree)
    {
      rightLeftSubtree->parent = root;
    }
    rightChild->parent = root->parent;
    root->parent = rightChild;
    updateHeight(root);
    updateHeight(rightChild);
    return rightChild;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::insert(Node_t* node, const Key& k, const Value& v, Node_t* parent)
  {
    if (!node)
    {
      return new Node_t(k, v, parent);
    }
    if (comp_(k, node->data.first))
    {
      node->left = insert(node->left, k, v, node);
    }
    else if (comp_(node->data.first, k))
    {
      node->right = insert(node->right, k, v, node);
    }
    else
    {
      node->data.second = v;
      return node;
    }
    updateHeight(node);
    int balance = getBalance(node);
    if (balance > 1 && comp_(k, node->left->data.first))
    {
      return rotateRight(node);
    }
    if (balance < -1 && comp_(node->right->data.first, k))
    {
      return rotateLeft(node);
    }
    if (balance > 1 && comp_(node->left->data.first, k))
    {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }
    if (balance < -1 && comp_(k, node->right->data.first))
    {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::find(Node_t* node, const Key& k) const
  {
    while (node)
    {
      if (comp_(k, node->data.first))
      {
        node = node->left;
      }
      else if (comp_(node->data.first, k))
      {
        node = node->right;
      }
      else
      {
        return node;
      }
    }
    return nullptr;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::findMin(Node_t* node) const
  {
    if (!node)
    {
      return nullptr;
    }
    while (node->left)
    {
      node = node->left;
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::findMax(Node_t* node) const
  {
    if (!node)
    {
      return nullptr;
    }
    while (node->right)
    {
      node = node->right;
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::remove(Node_t* node, const Key& k)
  {
    if (!node)
    {
      return nullptr;
    }
    if (comp_(k, node->data.first))
    {
      node->left = remove(node->left, k);
    }
    else if (comp_(node->data.first, k))
    {
      node->right = remove(node->right, k);
    }
    else
    {
      if (!node->left || !node->right)
      {
        Node_t* temp = node->left;
        if (!temp)
        {
          temp = node->right;
        }
        if (!temp)
        {
          temp = node;
          node = nullptr;
        }
        else
        {
          *node = *temp;
        }
        delete temp;
      }
      else
      {
        Node_t* temp = findMin(node->right);
        node->data = temp->data;
        node->right = remove(node->right, temp->data.first);
      }
    }
    if (!node)
    {
      return nullptr;
    }
    updateHeight(node);
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0)
    {
      return rotateRight(node);
    }
    if (balance > 1 && getBalance(node->left) < 0)
    {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0)
    {
      return rotateLeft(node);
    }
    if (balance < -1 && getBalance(node->right) > 0)
    {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }
    return node;
  }

  template < typename Key, typename Value, typename Compare >
  void Tree< Key, Value, Compare >::clear(Node_t* node)
  {
    if (node)
    {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t* Tree< Key, Value, Compare >::copyTree(Node_t* node, Node_t* parent)
  {
    if (!node)
    {
      return nullptr;
    }
    Node_t* newNode = new Node_t(node->data.first, node->data.second, parent);
    newNode->left = copyTree(node->left, newNode);
    newNode->right = copyTree(node->right, newNode);
    newNode->height = node->height;
    return newNode;
  }

  template < typename Key, typename Value, typename Compare >
  typename Tree< Key, Value, Compare >::Node_t*
    Tree< Key, Value, Compare >::findNextNode(Node_t* node) const
  {
    if (node == nullptr)
    {
      return nullptr;
    }
    if (node->right != nullptr)
    {
      node = node->right;
      while (node->left != nullptr)
      {
        node = node->left;
      }
      return node;
    }
    while (node->parent != nullptr)
    {
      if (node == node->parent->left)
      {
        return node->parent;
      }
      node = node->parent;
    }
    return nullptr;
  }

}

#endif
