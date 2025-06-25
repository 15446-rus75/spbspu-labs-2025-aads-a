  #ifndef TREE_HPP
  #define TREE_HPP
  #include <cassert>
  #include <initializer_list>
  #include <stdexcept>
  #include <type_traits>
  #include <utility>
  #include "iterator.hpp"
  #include "treeNode.hpp"

  namespace brevnov
  {
    template< typename Key, typename Value, typename Cmp = std::less< Key > >
    class AVLTree
    {
    public:
      using value = std::pair< Key, Value >;
      using Iter = Iterator< Key, Value, Cmp, false >;
      using ConstIter = Iterator< Key, Value, Cmp, true >;
      using IterPair = std::pair< Iter, Iter >;
      using ConstIterPair = std::pair< ConstIter, ConstIter >;
      using Node = TreeNode< Key, Value>;

      AVLTree();
      AVLTree(const AVLTree< Key, Value, Cmp >&);
      AVLTree(AVLTree< Key, Value, Cmp >&&) noexcept;
      AVLTree(std::initializer_list< value >);
      template< typename InputIt >
      AVLTree(InputIt first, InputIt last);
      ~AVLTree();

      AVLTree< Key, Value, Cmp >& operator=(const AVLTree< Key, Value, Cmp >&);
      AVLTree< Key, Value, Cmp >& operator=(AVLTree< Key, Value, Cmp >&&) noexcept;
      AVLTree< Key, Value, Cmp >& operator=(std::initializer_list< value >);
      Value& operator[](const Key&);
      const Value& operator[](const Key&) const;

      Value& at(const Key&);
      const Value& at(const Key&) const;

      size_t size() const noexcept;
      bool empty() const noexcept;

      Iter begin() noexcept;
      ConstIter cbegin() const noexcept;
      Iter end() noexcept;
      ConstIter cend() const noexcept;

      std::pair< Iter, bool > insert(const value&);
      std::pair< Iter, bool > insert(value&);
      std::pair< Iter, bool > insert(value&&);
      Iter insert(ConstIter, const value&);
      Iter insert(Iter, const value&);
      template< typename InputIt >
      void insert(InputIt first, InputIt last);
      void insert(std::initializer_list< value >);

      Iter erase(Iter) noexcept;
      Iter erase(ConstIter) noexcept;
      size_t erase(const Key&) noexcept;
      Iter erase(Iter first, Iter last) noexcept;
      Iter erase(ConstIter first, ConstIter last) noexcept;

      template< typename... Args >
      std::pair< Iter, bool > emplace(Args&&...);
      template< typename... Args >
      Iter emplaceHint(ConstIter, Args&&...);

      void swap(AVLTree< Key, Value, Cmp >&) noexcept;
      void clear() noexcept;

      Iter find(const Key&) noexcept;
      ConstIter find(const Key&) const noexcept;

      size_t count(const Key&) const noexcept;

      Iter lowerBound(const Key&) noexcept;
      ConstIter lowerBound(const Key&) const noexcept;
      Iter upperBound(const Key&) noexcept;
      ConstIter upperBound(const Key&) const noexcept;

      std::pair< Iter, Iter > equalRange(const Key&) noexcept;
      std::pair< ConstIter, ConstIter > equalRange(const Key&) const noexcept;

    private:
      int height(Node* node) const noexcept
      {
        if (node == nullptr)
        {
          return 0;
        }
        return node->nodeHeight;
      }

      int balanceFactor(Node* node) const noexcept
      {
        if (node == nullptr)
        {
          return 0;
        }
        return height(node->left) - height(node->right);
      }
      Node * leftRotate(Node* x) noexcept;
      Node * rightRotate(Node* y) noexcept;
      void fixHeight(Node* node) noexcept;
      Node * root_;
      Cmp cmp_;
      size_t size_;
    };

    template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >::AVLTree():
      root_(nullptr),
      size_(0)
    {}

    template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >::AVLTree(const AVLTree< Key, Value, Cmp >& tree):
      AVLTree()
    {
      for (ConstIter it = tree.cbegin(); it != tree.cend(); ++it)
      {
        insert(*it);
      }
    }

    template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >::AVLTree(AVLTree< Key, Value, Cmp >&& tree) noexcept:
      root_(std::exchange(tree.root_, nullptr)),
      size_(std::exchange(tree.size_, 0))
    {}

    template< typename Key, typename Value, typename Cmp >
    template< typename InputIt >
    AVLTree< Key, Value, Cmp >::AVLTree(InputIt first, InputIt last):
      AVLTree()
    {
      for (; first != last; first++)
      {
        insert(*first);
      }
    }

    template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >::AVLTree(std::initializer_list< value > il):
      AVLTree(il.begin(), il.end())
    {}

    template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >& AVLTree< Key, Value, Cmp >::operator=(const AVLTree< Key, Value, Cmp >& tree)
    {
      AVLTree< Key, Value, Cmp > cpy(tree);
      swap(cpy);
      return *this;
    }

    template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >& AVLTree< Key, Value, Cmp >::operator=(AVLTree< Key, Value, Cmp >&& tree) noexcept
    {
      AVLTree< Key, Value, Cmp > temp(std::move(tree));
      swap(temp);
      return *this;
    }

    template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >& AVLTree< Key, Value, Cmp >::operator=(std::initializer_list< value > il)
    {
      AVLTree< Key, Value, Cmp > temp(il);
      swap(temp);
      return *this;
    }

  template< typename Key, typename Value, typename Cmp >
    AVLTree< Key, Value, Cmp >::~AVLTree()
    {
      clear();
    }

    template< typename Key, typename Value, typename Cmp >
    void AVLTree< Key, Value, Cmp >::clear() noexcept
    {
      erase(begin(), end());
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Node* AVLTree< Key, Value, Cmp >::rightRotate(Node* y) noexcept
    {
      if (!y || !y->left)
      {
        return y;
      }
      Node* x = y->left;
      Node* T2 = x->right;
      x->right = y;
      y->left = T2;
      x->parent = y->parent;
      y->parent = x;
      if (T2)
      {
        T2->parent = y;
      }
      if (!x->parent)
      {
        root_ = x;
      }
      else if (x->parent->left == y)
      {
        x->parent->left = x;
      }
      else
      {
        x->parent->right = x;
      }
      y->nodeHeight = std::max(height(y->left), height(y->right)) + 1;
      x->nodeHeight = std::max(height(x->left), height(x->right)) + 1;
      return x;
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Node* AVLTree< Key, Value, Cmp >::leftRotate(Node* x) noexcept
    {
      if (!x || !x->right)
      {
        return x;
      }
      Node* y = x->right;
      Node* T2 = y->left;
      y->left = x;
      x->right = T2;
      y->parent = x->parent;
      x->parent = y;
      if (T2)
      {
        T2->parent = x;
      }
      if (!y->parent)
      {
        root_ = y;
      }
      else if (y->parent->left == x)
      {
        y->parent->left = y;
      }
      else
      {
        y->parent->right = y;
      }
      x->nodeHeight = std::max(height(x->left), height(x->right)) + 1;
      y->nodeHeight = std::max(height(y->left), height(y->right)) + 1;
      return y;
    }

    template< typename Key, typename Value, typename Cmp >
    size_t AVLTree< Key, Value, Cmp >::size() const noexcept
    {
      return size_;
    }

    template< typename Key, typename Value, typename Cmp >
    bool AVLTree< Key, Value, Cmp >::empty() const noexcept
    {
      return size_ == 0;
    }

    template< typename Key, typename Value, typename Cmp >
    void AVLTree< Key, Value, Cmp >::swap(AVLTree< Key, Value, Cmp >& tree) noexcept
    {
      std::swap(root_, tree.root_);
      std::swap(size_, tree.size_);
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::begin() noexcept
    {
      if (empty())
      {
        return end();
      }
      Node* temp = root_;
      while (temp->left)
      {
        temp = temp->left;
      }
      return Iter(temp, false);
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::ConstIter AVLTree< Key, Value, Cmp >::cbegin() const noexcept
    {
      if (empty())
      {
        return cend();
      }
      Node* temp = root_;
      while (temp->left)
      {
        temp = temp->left;
      }
      return ConstIter(temp, false);
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::end() noexcept
    {
      if (empty())
      {
        return Iter(root_, true);
      }
      Node* temp = root_;
      while (temp->right)
      {
        temp = temp->right;
      }
      return Iter(temp, true);
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::ConstIter AVLTree< Key, Value, Cmp >::cend() const noexcept
    {
      if (empty())
      {
        return ConstIter(root_, true);
      }
      Node* temp = root_;
      while (temp->right)
      {
        temp = temp->right;
      }
      return ConstIter(temp, true);
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::find(const Key& key) noexcept
    {
      Node* temp = root_;
      while (temp)
      {
        if (cmp_(key, temp->data.first))
        {
          temp = temp->left;
        }
        else if (cmp_(temp->data.first, key))
        {
          temp = temp->right;
        }
        else
        {
          return Iter(temp, false);
        }
      }
      return end();
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::ConstIter AVLTree< Key, Value, Cmp >::find(const Key& key) const noexcept
    {
      Node* temp = root_;
      while (temp)
      {
        if (cmp_(key, temp->data.first))
        {
          temp = temp->left;
        }
        else if (cmp_(temp->data.first, key))
        {
          temp = temp->right;
        }
        else
        {
          return ConstIter(temp, false);
        }
      }
      return cend();
    }

    template< typename Key, typename Value, typename Cmp >
    template< typename... Args >
    std::pair< typename AVLTree< Key, Value, Cmp >::Iter, bool > AVLTree< Key, Value, Cmp >::emplace(Args &&... args)
    {
      Node * newNode = nullptr;
      try
      {
        newNode = new Node{ nullptr, nullptr, nullptr,1 ,  { std::forward< Args >(args)... } };
        if (!root_)
        {
          root_ = newNode;
          size_ = 1;
          return { Iter(root_, false), true };
        }
        Node* temp = root_;
        Node* parent = nullptr;
        while (temp)
        {
          parent = temp;
          if (cmp_(newNode->data.first, temp->data.first))
          {
            temp = temp->left;
          }
          else if (cmp_(temp->data.first, newNode->data.first))
          {
            temp = temp->right;
          }
          else
          {
            delete newNode;
            return { Iter(temp, false), false };
          }
        }
        newNode->parent = parent;
        if (cmp_(parent->data.first, newNode->data.first))
        {
          parent->right = newNode;
        }
        else
        {
          parent->left = newNode;
        }
        fixHeight(root_);
        size_++;
        return { Iter(newNode, false), true };
      }
      catch (...)
      {
        if (newNode)
        {
          delete newNode;
        }
        throw;
      }
    }

    template< typename Key, typename Value, typename Cmp >
    void AVLTree< Key, Value, Cmp >::fixHeight(Node* node) noexcept
    {
      if (node == nullptr)
      {
        return;
      }
      fixHeight(node->left);
      fixHeight(node->right);
      node->nodeHeight = 1 + std::max(height(node->left), height(node->right));
      int balance = balanceFactor(node);
      if (balance > 1 && cmp_(node->data.first, node->left->data.first))
      {
        node = rightRotate(node);
      }
      if (balance < -1 && !cmp_(node->data.first, node->right->data.first))
      {
        node = leftRotate(node);
      }
      if (balance > 1 && !cmp_(node->data.first, node->left->data.first))
      {
        node->left = leftRotate(node->left);
        node = rightRotate(node);
      }
      if (balance < -1 && cmp_(node->data.first, node->right->data.first))
      {
        node->right = rightRotate(node->right);
        node = leftRotate(node);
      }
    }

    template< typename Key, typename Value, typename Cmp >
    template< typename... Args >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::emplaceHint(ConstIter hint, Args &&... args)
    {
      Node * newNode = nullptr;
      if (hint == cend() || empty())
      {
        return emplace(std::forward< Args >(args)...).first;
      }
      try
      {
        Node* pos = hint.node_;
        newNode = new Node{nullptr, nullptr, pos, 1,{ std::forward< Args >(args)... } };
        value val = newNode->data;
        if (cmp_(val.first, pos->data.first))
        {
          if (!pos->left)
          {
            pos->left = newNode;
            fixHeight(root_);
            ++size_;
            return Iter(newNode, false);
          }
        }
        else if (cmp_(pos->data.first, val.first))
        {
          if (!pos->right)
          {
            pos->right = newNode;
            fixHeight(root_);
            ++size_;
            return Iter(newNode, false);
          }
        }
        else
        {
          delete newNode;
          return Iter(pos, false);
        }
        delete newNode;
        return emplace(val).first;
      }
      catch (...)
      {
        if (newNode)
        {
          delete newNode;
        }
        throw;
      }
    }

    template< typename Key, typename Value, typename Cmp >
    std::pair< typename AVLTree< Key, Value, Cmp >::Iter, bool > AVLTree< Key, Value, Cmp >::insert(const value& val)
    {
      return emplace(val);
    }

    template< typename Key, typename Value, typename Cmp >
    std::pair< typename AVLTree< Key, Value, Cmp >::Iter, bool > AVLTree< Key, Value, Cmp >::insert(value& val)
    {
      return emplace(val);
    }

    template< typename Key, typename Value, typename Cmp >
    std::pair< typename AVLTree< Key, Value, Cmp >::Iter, bool > AVLTree< Key, Value, Cmp >::insert(value&& val)
    {
      return emplace(std::move(val));
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::insert(ConstIter pos, const value& val)
    {
      return emplaceHint(pos, val);
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::insert(Iter pos, const value& val)
    {
      ConstIter it(pos);
      return emplaceHint(it, val);
    }

    template< typename Key, typename Value, typename Cmp >
    template< typename InputIt >
    void AVLTree< Key, Value, Cmp >::insert(InputIt first, InputIt last)
    {
      AVLTree< Key, Value, Cmp > temp(*this);
      for (; first != last; ++first)
      {
        temp.insert(*first);
      }
      swap(temp);
    }

    template< typename Key, typename Value, typename Cmp >
    void AVLTree< Key, Value, Cmp >::insert(std::initializer_list< value > il)
    {
      insert(il.begin(), il.end());
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::erase(ConstIter pos) noexcept
    {
      if (pos == cend())
      {
        return end();
      }
      Node* del = pos.node_;
      Node* parent = del->parent;
      Iter next(pos.node_, pos.isEnd_);
      next++;
      if (!del->left && !del->right)
      {
        if (parent)
        {
          if (parent->left == del)
          {
            parent->left = nullptr;
          }
          else
          {
            parent->right = nullptr;
          }
        }
        else
        {
          root_ = nullptr;
          size_ = 0;
          return end();
        }
        delete del;
        size_--;
        fixHeight(root_);
        return next;
      }
      if (!del->left || !del->right)
      {
        Node* child = del->left ? del->left : del->right;
        if (parent)
        {
          if (parent->left == del)
          {
            parent->left = child;
          }
          else
          {
            parent->right = child;
          }
        }
        else
        {
          root_ = child;
        }
        if (child)
        {
          child->parent = parent;
        }
        delete del;
        size_--;
        fixHeight(root_);
        return next;
      }
      Node* successor = del->right;
      while (successor->left)
      {
        successor = successor->left;
      }
      Node* succParent = successor->parent;
      Node* succChild = successor->right;
      std::swap(del->data, successor->data);
      if (succParent->left == successor)
      {
        succParent->left = succChild;
      }
      else
      {
        succParent->right = succChild;
      }
      if (succChild)
      {
        succChild->parent = succParent;
      }
      delete successor;
      size_--;
      fixHeight(root_);
      return next;
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::erase(Iter pos) noexcept
    {
      ConstIter it(pos);
      return erase(it);
    }

    template< typename Key, typename Value, typename Cmp >
    size_t AVLTree< Key, Value, Cmp >::erase(const Key& key) noexcept
    {
      Iter it = find(key);
      if (it == end())
      {
        return 0;
      }
      erase(it);
      return 1;
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::erase(ConstIter first, ConstIter last) noexcept
    {
      while (first != last)
      {
        first = erase(first);
      }
      return Iter(last.node_, last.isEnd_);
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::erase(Iter first, Iter last) noexcept
    {
      ConstIter constFirst(first);
      ConstIter constLast(last);
      return erase(constFirst, constLast);
    }

    template< typename Key, typename Value, typename Cmp >
    size_t AVLTree< Key, Value, Cmp >::count(const Key& key) const noexcept
    {
      ConstIter it = find(key);
      return it == cend() ? 0 : 1;
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::lowerBound(const Key& key) noexcept
    {
      Node* temp = root_;
      Node* result = nullptr;
      while (temp)
      {
        if (!cmp_(temp->data.first, key))
        {
          result = temp;
          temp = temp->left;
        }
        else
        {
          temp = temp->right;
        }
      }
      return result ? Iter(result, false) : end();
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::ConstIter AVLTree< Key, Value, Cmp >::lowerBound(const Key& key) const noexcept
    {
      return ConstIter(lowerBound(key));
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::Iter AVLTree< Key, Value, Cmp >::upperBound(const Key& key) noexcept
    {
      Node* temp = root_;
      Node* result = nullptr;
      while (temp)
      {
        if (cmp_(key, temp->data.first))
        {
          result = temp;
          temp = temp->left;
        }
        else
        {
          temp = temp->right;
        }
      }
      return result ? Iter(result, false) : end();
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::ConstIter AVLTree< Key, Value, Cmp >::upperBound(const Key& key) const noexcept
    {
      return ConstIter(upperBound(key));
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::IterPair AVLTree< Key, Value, Cmp >::equalRange(const Key& key) noexcept
    {
      return { Iter(lowerBound(key)), Iter(upperBound(key)) };
    }

    template< typename Key, typename Value, typename Cmp >
    typename AVLTree< Key, Value, Cmp >::ConstIterPair AVLTree< Key, Value, Cmp >::equalRange(const Key& key) const noexcept
    {
      return { ConstIter(lowerBound(key)), ConstIter(upperBound(key)) };
    }

    template< typename Key, typename Value, typename Cmp >
    const Value& AVLTree< Key, Value, Cmp >::operator[](const Key& key) const
    {
      ConstIter it = find(key);
      return it->second;
    }

    template< typename Key, typename Value, typename Cmp >
    Value& AVLTree< Key, Value, Cmp >::operator[](const Key& key)
    {
      Iter it = insert(std::make_pair(key, Value())).first;
      return it->second;
    }

    template< typename Key, typename Value, typename Cmp >
    const Value& AVLTree< Key, Value, Cmp >::at(const Key& key) const
    {
      ConstIter it = find(key);
      if (it == cend())
      {
        throw std::out_of_range("There is no such key");
      }
      return it->second;
    }

    template< typename Key, typename Value, typename Cmp >
    Value& AVLTree< Key, Value, Cmp >::at(const Key& key)
    {
      return const_cast< Value& >(static_cast< const AVLTree< Key, Value, Cmp >& >(*this).at(key));
    }
  }
  #endif
