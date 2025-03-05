#ifndef LIST_HPP
#define LIST_HPP
#include <initializer_list>
#include <new>
#include <stdexcept>
#include <utility>
#include "constIterator.hpp"
#include "node.hpp"

namespace kiselev
{
  template< typename T>
  class List
  {
  public:

    List();
    List(const List< T >&);
    List(List< T >&&) noexcept;
    List(size_t, const T&);
    template< typename InputIterator >
    List< T >(InputIterator first, InputIterator last);
    List< T >(std::initializer_list< T >);
    ~List();

    List< T >& operator=(const List< T >&);
    List< T >& operator=(List< T >&&) noexcept;
    List< T >& operator=(std::initializer_list< T >);

    bool operator==(const List< T >&) const noexcept;
    bool operator!=(const List< T >&) const noexcept;
    bool operator<(const List< T >&) const noexcept;
    bool operator>(const List< T >&) const noexcept;
    bool operator<=(const List< T >&) const noexcept;
    bool operator>=(const List< T >&) const noexcept;

    Iterator< T > begin() noexcept;
    ConstIterator< T > cbegin() const noexcept;
    Iterator< T > end() noexcept;
    ConstIterator< T > cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;

    void pushBack(const T);
    void pushFront(const T);

    void popBack() noexcept;
    void popFront() noexcept;

    void clear() noexcept;
    void swap(List< T >&) noexcept;

    void assign(size_t, const T&);
    template< typename InputIterator >
    void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list< T >);

    Iterator< T > erase(ConstIterator< T >) noexcept;
    Iterator< T > erase(ConstIterator< T > first, ConstIterator< T > last) noexcept;

    Iterator< T > insert(ConstIterator< T >, const T&);
    Iterator< T > insert(ConstIterator< T >, T&&);
    Iterator< T > insert(ConstIterator< T >, size_t, const T&);
    template< typename InputIterator >
    Iterator< T > insert(ConstIterator< T > pos, InputIterator first, InputIterator last);
    Iterator< T > insert(ConstIterator< T >, std::initializer_list< T >);

    void remove(const T&) noexcept;
    template< typename Predicate >
    void remove_if(Predicate) noexcept;

    void splice(ConstIterator< T >, List< T >&) noexcept;
    void splice(ConstIterator< T >, List< T >&&) noexcept;
    void splice(ConstIterator< T > position, List< T >&, ConstIterator< T > i) noexcept;
    void splice(ConstIterator< T > position, List< T >&&, ConstIterator< T > i) noexcept;
    void splice(ConstIterator< T > pos, List< T >&, ConstIterator< T > first, ConstIterator< T > last) noexcept;
    void splice(ConstIterator< T > pos, List< T >&&, ConstIterator< T > first, ConstIterator< T > last) noexcept;
    void reverse() noexcept;


  private:

    Node< T >* head_;
    Node< T >* end_;
    size_t size_;

  };

  template< typename T >
  List< T >::List():
    head_(nullptr),
    end_(nullptr),
    size_(0)
  {
  }

  template< typename T>
  List< T >::List(const List< T >& list):
    List()
  {
    for (ConstIterator< T > it = list.cbegin(); it != list.cend(); ++it)
    {
      pushBack(*it);
    }
  }

  template< typename T >
  List< T >::List(List< T >&& list) noexcept:
    head_(std::exchange(list.head_, nullptr)),
    end_(std::exchange(list.end_, new Node< T >())),
    size_(std::exchange(list.size_, 0))
  {
    list.end_->next_ = list.end_;
    list.end_->prev_ = list.end_;
  }

  template< typename T >
  List< T >::List(size_t count, const T& data):
    List()
  {
    for (size_t i = 0; i < count; ++i)
    {
      pushBack(data);
    }
  }

  template< typename T >
  template< typename InputIterator >
  List< T >::List(InputIterator first, InputIterator last):
    List()
  {
    for (; first != last; ++first)
    {
      pushFront(*first);
    }
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > il):
    List(il.begin(), il.end())
  {}

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete end_;
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& list)
  {
    List< T > cpy(list);
    swap(cpy);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& list) noexcept
  {
    List< T > temp(std::move(list));
    swap(temp);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > temp(il);
    swap(temp);
    return *this;
  }

  template< typename T >
  bool List< T >::operator==(const List<T>& list) const noexcept
  {
    if (size_ == list.size_)
    {
      ConstIterator< T > thisIt = cbegin();
      ConstIterator< T > otherIt = list.cbegin();
      for (; thisIt != cend(); ++thisIt, ++otherIt)
      {
        if (*thisIt != *otherIt)
        {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  template< typename T >
  bool List< T >::operator!=(const List<T>& list) const noexcept
  {
    return !(*this == list);
  }

  template< typename T >
  bool List< T >::operator<(const List< T >& list) const noexcept
  {
    size_t min = std::min(size_, list.size_);
    ConstIterator< T > thisIt = cbegin();
    ConstIterator< T > otherIt = list.cbegin();
    for (size_t i = 0; i < min; ++i, ++thisIt, ++otherIt)
    {
      if (*thisIt != *otherIt)
      {
        return *thisIt < * otherIt;
      }
    }
    return false;
  }

  template< typename T >
  bool List< T >::operator>(const List< T >& list) const noexcept
  {
    return list < *this;
  }

  template< typename T >
  bool List< T >::operator<=(const List< T >& list) const noexcept
  {
    return !(*this > list);
  }

  template< typename T >
  bool List< T >::operator>=(const List<T>& list) const noexcept
  {
    return !(*this < list);
  }

  template< typename T >
  Iterator< T > List< T >::begin() noexcept
  {
    return Iterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return ConstIterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return ConstIterator< T >(empty() ? head_ : end_);
  }

  template< typename T >
  Iterator< T > List< T >::end() noexcept
  {
    return Iterator< T >(empty() ? head_ : end_);
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    return head_->data_;
  }

  template< typename T >
  const T& List< T >::front() const noexcept
  {
    return head_->data_;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    return end_->prev_->data_;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    return end_->prev_->data_;
  }

  template< typename T >
  void List< T >::pushBack(const T data)
  {
    insert(cend(), data);
  }

  template< typename T >
  void List< T >::pushFront(const T data)
  {
    insert(cbegin(), data);
  }

  template< typename T >
  void List< T >::popBack() noexcept
  {
    assert(!empty());
    erase(ConstIterator< T >(end_->prev_));
  }

  template< typename T >
  void List< T >::popFront() noexcept
  {
    assert(!empty());
    erase(cbegin());
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      popFront();
    }
  }

  template< typename T >
  void List< T >::swap(List<T>& list) noexcept
  {
    std::swap(head_, list.head_);
    std::swap(end_, list.end_);
    std::swap(size_, list.size_);
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& data)
  {
    List< T > temp(count, data);
    swap(temp);
  }

  template< typename T >
  template< typename InputIterator >
  void List< T >::assign(InputIterator first, InputIterator last)
  {
    List< T > temp(first, last);
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    List< T > temp(il);
    swap(temp);
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > position) noexcept
  {
    Iterator< T > it(position.node_->next_);
    if (position == cbegin())
    {
      head_ = head_->next_;
    }
    else if (position.node_ == end_->prev_)
    {
      end_->prev_ = position.node_->prev_;
    }
    position.node_->prev_->next_ = position.node_->next_;
    position.node_->next_->prev_ = position.node_->prev_;
    delete position.node_;
    size_--;
    return it;
  }

  template< typename T >
  Iterator< T > List< T >::erase(ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    for (; first != last;)
    {
      first = erase(first);
    }
    return Iterator< T >(last.node_);
  }

  template< typename T >
  void List< T >::remove(const T& data) noexcept
  {
    if (size_ == 1)
    {
      if (head_->data_ == data)
      {
        popFront();
      }
    }
    else
    {
      for (ConstIterator< T > it = cbegin(); it != cend();)
      {
        if (it.node_->data_ == data)
        {
          it = erase(it);
        }
        else
        {
          it++;
        }
      }
    }
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred) noexcept
  {
    for (ConstIterator< T > it = cbegin(); it != cend();)
    {
      if (pred(*it))
      {
        it = erase(it);
      }
      else
      {
        ++it;
      }
    }
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >& list) noexcept
  {
    splice(position, list, list.cbegin(), list.cend());
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >&& list) noexcept
  {
    splice(position, list);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >& list, ConstIterator< T > i) noexcept
  {
    splice(position, list, i, std::next(i));
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > position, List< T >&& list, ConstIterator< T > i) noexcept
  {
    splice(position, list, i);
  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >& list, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    Node< T >* firstNode = first.node_;
    Node< T >* lastNode = last.node_->prev_;
    size_t distance = std::distance(first, last);
    list.size_ -= distance;
    size_ += distance;
    firstNode->prev_->next_ = lastNode->next_;
    lastNode->next_->prev_ = firstNode->prev_;
    if (firstNode == list.head_)
    {
      list.head_ = last.node_;
      list.head_->prev_ = end_;
      list.end_->next_ = list.head_;
    }
    if (lastNode == list.end_->prev_)
    {
      list.end_->prev_ = first.node_->prev_;
      first.node_->prev_->next_ = list.end_;
    }
    if (pos == cbegin())
    {
      lastNode->next_ = head_;
      head_->prev_ = lastNode;
      head_ = firstNode;
      head_->prev_ = end_;
      end_->next_ = head_;
    }
    else
    {
      pos.node_->prev_->next_ = firstNode;
      firstNode->prev_ = pos.node_->prev_;
      pos.node_->prev_ = lastNode;
      lastNode->next_ = pos.node_;
    }


  }

  template< typename T >
  void List< T >::splice(ConstIterator< T > pos, List< T >&& list, ConstIterator< T > first, ConstIterator< T > last) noexcept
  {
    splice(pos, list, first, last);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator<T> position, const T& data)
  {
    Node< T >* node = new Node< T >(data);
    if (empty())
    {
      try
      {
        end_ = new Node< T >();
      }
      catch (const std::bad_alloc&)
      {
        delete node;
        throw;
      }
      node->next_ = end_;
      node->prev_ = end_;
      end_->prev_ = node;
      end_->next_ = node;
      head_ = node;
    }
    else if (position == cbegin())
    {
      node->next_ = head_;
      node->prev_ = end_;
      head_->prev_ = node;
      end_->next_ = node;
      head_ = node;
    }
    else
    {
      node->next_ = position.node_;
      node->prev_ = position.node_->prev_;
      position.node_->prev_->next_ = node;
      position.node_->prev_ = node;
    }
    ++size_;
    return Iterator< T >(node);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > position, T&& data)
  {
    return insert(position, data);
  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > position, size_t n, const T& data)
  {
    if (n == 0)
    {
      return Iterator< T >(position.node_);
    }
    Iterator< T > res = insert(position, data);
    size_t i = 1;
    try
    {
      for (; i < n; ++i)
      {
        insert(position, data);
      }
      return Iterator< T >(res.node_);
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        position = erase(--position);
      }
      throw;
    }
  }

  template< typename T >
  template< typename InputIterator >
  Iterator< T > List< T >::insert(ConstIterator< T > pos, InputIterator first, InputIterator last)
  {
    if (first == last)
    {
      return Iterator< T >(pos.node_);
    }
    Iterator< T > res = insert(pos, *first);
    ++first;
    size_t i = 1;
    try
    {
      for (; first != last; ++first)
      {
        insert(pos, *first);
        ++i;
      }
      return res;
    }
    catch (const std::bad_alloc&)
    {
      for (size_t j = 0; j < i; ++j)
      {
        pos = erase(--pos);
      }
      throw;
    }

  }

  template< typename T >
  Iterator< T > List< T >::insert(ConstIterator< T > position, std::initializer_list< T > il)
  {
    return insert(position, il.begin(), il.end());
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (empty() || size_ == 1)
    {
      return;
    }
    Node< T >* node = head_;
    head_ = end_->prev_;
    end_->prev_ = node;
    ConstIterator< T > it = head_;
    for (; it != cend(); ++it)
    {
      node = it.node_->next_;
      it.node_->next_ = it.node_->prev_;
      it.node_->prev_ = node;
    }
  }



}
#endif
