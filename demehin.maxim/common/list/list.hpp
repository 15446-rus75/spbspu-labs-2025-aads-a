#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include "iterator.hpp"
#include "cIterator.hpp"
#include "node.hpp"

namespace
{
  template< typename T >
  void setOtherPos(demehin::cListIterator< T >& otherPos, const demehin::cListIterator< T >& begin,
    const demehin::cListIterator< T >& pos)
  {
    for (auto it = begin; it != pos; ++it)
    {
      otherPos++;
    }
  }
}

namespace demehin
{
  template< typename T >
  struct List
  {
  public:
    using Iter = ListIterator< T >;
    using cIter = cListIterator< T >;

    List();
    List(const List< T >&);
    List(List< T >&&);

    List(size_t, const T&);
    List(std::initializer_list< T >);
    List(Iter, Iter);

    ~List();

    List< T >& operator=(const List< T >&);
    bool operator==(const List< T >&) const noexcept;
    bool operator!=(const List< T >&) const noexcept;
    bool operator<(const List< T >&) const noexcept;
    bool operator<=(const List< T >&) const noexcept;
    bool operator>(const List< T >&) const noexcept;
    bool operator>=(const List< T >&) const noexcept;

    Iter begin() const noexcept;
    Iter end() const noexcept;

    cIter cbegin() const noexcept;
    cIter cend() const noexcept;

    size_t size() const noexcept;

    void pop_front() noexcept;
    void pop_back() noexcept;

    void push_front(const T&);
    void push_back(const T&);

    T& front() noexcept;
    T& back() noexcept;

    bool empty() noexcept;

    void clear() noexcept;

    void swap(List&) noexcept;

    void splice(cIter pos, List< T >&) noexcept;
    void splice(cIter pos, List< T >&&) noexcept;
    void splice(cIter pos, List< T >&, cIter it) noexcept;
    void splice(cIter pos, List< T >&&, cIter it) noexcept;
    void splice(cIter pos, List< T >&, cIter first, cIter last) noexcept;
    void splice(cIter pos, List< T >&&, cIter first, cIter last) noexcept;

    void fill(Iter first, Iter last, const T&) noexcept;

    void remove(const T&) noexcept;
    template< typename UnaryPredicate >
    void removeIf(UnaryPredicate) noexcept;

    void assign(size_t, const T&);
    void assign(Iter, Iter);
    void assign(std::initializer_list < T >);

    Iter erase(cIter pos) noexcept;
    Iter erase(cIter first, cIter last) noexcept;

    Iter insert(cIter, const T&);
    Iter insert(cIter, T&&);
    Iter insert(cIter, size_t, const T&);
    template< typename InputIt >
    Iter insert(cIter, InputIt, InputIt);
    Iter insert(cIter, std::initializer_list< T >);

    void reverse();

  private:
    using Node = demehin::Node< T >;

    Node* fake_;
    Node* tail_;
    size_t size_;

  };

  template< typename T >
  using Node = demehin::Node< T >;

  template< typename T >
  List< T >::List():
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    tail_(nullptr),
    size_(0)
  {
    fake_->next = nullptr;
    fake_->prev = nullptr;
  }

  template< typename T >
  List< T >::List(size_t count, const T& value):
    List()
  {
    assign(count, value);
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > ilist):
    List()
  {
    assign(ilist);
  }

  template< typename T >
  List< T >::List(ListIterator< T > first, ListIterator< T > last):
    List()
  {
    assign(first, last);
  }

  template< typename T >
  List< T >::~List()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_);
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    assign(other.begin(), other.end());
    return *this;
  }

  template< typename T >
  bool List< T >::operator==(const List< T >& rhs) const noexcept
  {
    if (size() != rhs.size())
    {
      return false;
    }

    auto it1 = begin();
    auto it2 = rhs.begin();
    while (it1 != end())
    {
      if (*it1 != *it2)
      {
        return false;
      }
      ++it1;
      ++it2;
    }
    return true;
  }

  template< typename T >
  bool List< T >::operator!=(const List< T >& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool List< T >::operator<(const List< T >& rhs) const noexcept
  {
    auto it1 = begin();
    auto it2 = rhs.begin();
    while (it1 != end() && it2 != rhs.end())
    {
      if (*it1 < *it2)
      {
        return true;
      }
      if (*it1 > *it2)
      {
        return false;
      }
      it1++;
      it2++;
    }

    return size() < rhs.size();
  }

  template< typename T >
  bool List< T >::operator<=(const List< T >& rhs) const noexcept
  {
    return *this < rhs || *this == rhs;
  }

  template< typename T >
  bool List< T >::operator>(const List< T >& rhs) const noexcept
  {
    return !(*this <= rhs);
  }

  template< typename T >
  bool List< T >::operator>=(const List< T >& rhs) const noexcept
  {
    return *this > rhs || *this == rhs;
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_back();
    }
  }

  template< typename T >
  List< T >::List(const List< T >& rhs):
    List()
  {
    Node* current = rhs.fake_->next;
    while (current != nullptr)
    {
      push_back(current->data);
      current = current->next;
    }
  }

  template< typename T >
  List< T >::List(List< T >&& rhs):
    fake_(rhs.fake_),
    tail_(rhs.tail_),
    size_(rhs.size_)
  {
    std::exchange(rhs.fake_, nullptr);
    std::exchange(rhs.tail_, nullptr);
    std::exchange(rhs.size_, 0);
  }

  template< typename T >
  ListIterator< T > List< T >::begin() const noexcept
  {
    return ListIterator< T >(fake_->next);
  }

  template< typename T >
  ListIterator< T > List< T >::end() const noexcept
  {
    return ListIterator< T >(tail_->next);
  }

  template< typename T >
  cListIterator< T > List< T >::cbegin() const noexcept
  {
    return cListIterator< T >(fake_->next);
  }

  template< typename T >
  cListIterator< T > List< T >::cend() const noexcept
  {
    return cListIterator< T >(tail_->next);
  }

  template< typename T >
  void List< T >::pop_front() noexcept
  {
    Node* todelete = fake_->next;
    fake_->next = todelete->next;
    if (fake_->next != nullptr)
    {
      fake_->next->prev = fake_;
    }
    else
    {
      tail_ = nullptr;
    }
    delete todelete;
    size_--;
  }

  template< typename T >
  void List< T >::pop_back() noexcept
  {
    Node* todelete = tail_;
    tail_ = tail_->prev;

    if (tail_ != nullptr)
    {
      tail_->next = nullptr;
    }
    else
    {
      fake_->next = nullptr;
    }

    delete todelete;
    size_--;
  }

  template< typename T >
  void List< T >::push_back(const T& data)
  {
    Node* new_node = new Node(data);
    new_node->next = nullptr;
    new_node->prev = tail_;
    if (tail_ != nullptr)
    {
      tail_->next = new_node;
    }
    else
    {
      fake_->next = new_node;
      new_node->prev = fake_;
    }

    tail_ = new_node;
    size_++;
  }

  template< typename T >
  void List< T >::push_front(const T& data)
  {
    Node* new_node = new Node(data);
    new_node->next = fake_->next;

    if (fake_->next != nullptr)
    {
      fake_->next->prev = new_node;
    }
    else
    {
      tail_ = new_node;
    }

    new_node->prev = fake_;
    fake_->next = new_node;
    size_++;
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    return fake_->next->data;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    return tail_->data;
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  bool List< T >::empty() noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

  template< typename T >
  void List< T >::fill(ListIterator< T > first, ListIterator< T > last, const T& value) noexcept
  {
    for (; first != last; ++first)
    {
      *first = value;
    }
  }

  template< typename T >
  void List< T >::splice(cListIterator< T > pos, List< T >& other) noexcept
  {
    if (other.empty())
    {
      return;
    }

    Node* first = other.fake_->next;
    Node* last = other.tail_;
    Node* posNode = pos.getNode();
    Node* posPrev = posNode->prev;

    if (posPrev != nullptr)
    {
      posPrev->next = first;
    }

    first->prev = posPrev;

    if (posNode != nullptr)
    {
      posNode->prev = last;
    }

    last->next = posNode;

    if (fake_->next == posNode)
    {
      fake_->next = first;
    }
    if (tail_ == posPrev)
    {
      tail_ = last;
    }

    size_ += other.size();
    std::exchange(other.size_, 0);
    std::exchange(other.fake_, nullptr);
    std::exchange(other.tail_, nullptr);
  }

  template< typename T >
  void List< T >::splice(cListIterator< T > pos, List< T >&& other) noexcept
  {
    List< T > tempList(std::move(other));
    splice(tempList);
  }

  template< typename T >
  void List< T >::splice(cListIterator< T > pos, List< T >& other, cListIterator< T > it) noexcept
  {
    if (other.empty() || pos == cend() || it == other.cbegin())
    {
      return;
    }

    Node* itNode = it.getNode();
    Node* posNode = pos.getNode();
    Node* itPrevNode = itNode->prev;

    itPrevNode->next = itNode->next;
    itNode->prev = posNode->prev;
    itNode->next = posNode;
    posNode->prev->next = itNode;
    posNode->prev = itNode;
    size_++;
    other.size_--;
  }

  template< typename T >
  void List< T >::splice(cListIterator< T > pos, List< T >&& other, cListIterator< T > it) noexcept
  {
    List< T > tempList(std::move(other));
    splice(tempList);
  }

  template< typename T >
  void List< T >::splice(cListIterator< T > pos, List< T >& other, cListIterator< T > first,
    cListIterator< T > last) noexcept
  {
    if (other.empty() || pos == cend())
    {
      return;
    }

    Node* posNode = pos.getNode();
    Node* firstNode = first.getNode();
    Node* lastNode = last.getNode();
    Node* posPrevNode = posNode->prev;
    Node* lastNextNode = lastNode->next;

    size_t node_cnt = 1;
    for (Node* current = firstNode; current != lastNode; current = current->next)
    {
      node_cnt++;
    }

    if (lastNextNode != nullptr)
    {
      lastNextNode->prev = firstNode->prev;
    }

    if (posPrevNode != nullptr)
    {
      posPrevNode->next = firstNode;
    }
    else
    {
      fake_->next = firstNode;
    }

    firstNode->prev = posPrevNode;
    lastNode->next = posNode;
    posNode->prev = lastNode;

    size_ += node_cnt;
    other.size_ -= node_cnt;
  }

  template< typename T >
  void List< T >::splice(cListIterator< T > pos, List< T >&& other, cListIterator< T > first,
    cListIterator< T > last) noexcept
  {
    List< T > tempList(std::move(other));
    splice(tempList);
  }

  template< typename T >
  void List< T >::remove(const T& value) noexcept
  {
    for (auto it = cbegin(); it != cend(); it++)
    {
      if (*it == value)
      {
        erase(it);
      }
    }
  }

  template< typename T >
  template< typename UnaryPredicate >
  void List< T >::removeIf(UnaryPredicate p) noexcept
  {
    for (auto it = cbegin(); it != cend(); it++)
    {
      if (p(*it))
      {
        erase(it);
      }
    }
  }

  template< typename T >
  void List< T >::assign(size_t count, const T& value)
  {
    List< T > temp;
    for (size_t i = 0; i < count; i++)
    {
      temp.push_back(value);
    }
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(ListIterator< T > first, ListIterator< T > last)
  {
    List< T > temp;
    for (auto it = first; it != last; it++)
    {
      temp.push_back(*it);
    }
    swap(temp);
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > ilist)
  {
    List< T > temp;
    for (const T& value : ilist)
    {
      temp.push_back(value);
    }
    swap(temp);
  }

  template< typename T >
  ListIterator< T > List< T >::erase(cListIterator< T > pos) noexcept
  {
    const Node* todelete = pos.getNode();
    ListIterator< T > toreturn(todelete->next);
    if (todelete == fake_->next)
    {
      fake_->next = todelete->next;
    }
    else
    {
      todelete->prev->next = todelete->next;
    }

    if (todelete == tail_)
    {
      tail_ = todelete->prev;
    }
    else
    {
      todelete->next->prev = todelete->prev;
    }
    size_--;
    delete todelete;
    return toreturn;
  }

  template< typename T >
  ListIterator< T > List< T >::erase(cListIterator< T > first, cListIterator< T > last) noexcept
  {
    ListIterator< T > toreturn;
    for (auto it = first; it != last; it++)
    {
      toreturn = erase(it);
    }
    return toreturn;
  }

  template< typename T >
  ListIterator< T > List< T >::insert(cListIterator< T > pos, const T& value)
  {
    if (empty())
    {
      push_back(value);
      return begin();
    }

    Node* newNode = new Node(value);
    Node* posNode = pos.getNode();
    newNode->next = posNode;
    newNode->prev = posNode->prev;
    posNode->prev->next = newNode;
    posNode->prev = newNode;
    size_++;
    return ListIterator< T >(newNode);
  }

  template< typename T >
  ListIterator< T > List< T >::insert(cListIterator< T > pos, T&& value)
  {
    T temp_val = std::move(value);
    return insert(pos, temp_val);
  }

  template< typename T >
  ListIterator< T > List< T >::insert(cListIterator< T > pos, size_t count, const T& value)
  {
    if (count == 0)
    {
      return ListIterator< T >(pos.getNode());
    }

    List< T > temp(*this);
    cListIterator< T > tempPos = temp.cbegin();
    setOtherPos(tempPos, cbegin(), pos);
    ListIterator< T > toreturn = temp.insert(tempPos, value);
    for (size_t i = 1; i < count; i++)
    {
      temp.insert(tempPos, value);
    }
    swap(temp);
    return toreturn;
  }

  template< typename T >
  template< typename InputIt >
  ListIterator< T > List< T >::insert(cListIterator< T > pos, InputIt first, InputIt last)
  {
    if (first == last)
    {
      return ListIterator< T >(pos.getNode());
    }

    List< T > temp(*this);
    cListIterator< T > tempPos = temp.cbegin();
    setOtherPos(tempPos, cbegin(), pos);
    ListIterator< T > toreturn;
    for (auto it = first; it != last; it++)
    {
      toreturn = temp.insert(tempPos, *it);
    }
    swap(temp);
    return toreturn;
  }

  template< typename T >
  ListIterator< T > List< T >::insert(cListIterator< T > pos, std::initializer_list< T > ilist)
  {
    if (ilist.size() == 0)
    {
      return ListIterator< T >(pos.getNode());
    }

    return insert(pos, ilist.begin(), ilist.end());
  }

  template< typename T >
  void List< T >::reverse()
  {
    List < T > temp;
    for (auto it = begin(); it != end(); it++)
    {
      temp.push_front(*it);
    }
    *this = temp;
  }
}

#endif
