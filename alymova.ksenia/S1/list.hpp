#ifndef LIST_HPP
#define LIST_HPP
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>

namespace alymova
{
  template< typename T >
  struct ListNode;
  template< typename T >
  struct Iterator;
  template< typename T >
  struct ConstIterator;

  template< typename T >
  class List
  {
  public:
    List();
    List(const List< T >& other);
    List(List< T >&& other) noexcept;
    List(size_t n, const T& value = T());
    template< typename InputIterator>
    List(InputIterator first, InputIterator last);
    List(std::initializer_list< T > il);
    ~List() noexcept;

    List< T >& operator=(const List< T >& other);
    List< T >& operator=(List< T >&& other) noexcept;
    List< T >& operator=(std::initializer_list< T > il);

    Iterator< T > begin() noexcept;
    ConstIterator< T > begin() const noexcept;
    ConstIterator< T > cbegin() const noexcept;
    Iterator< T > end() noexcept;
    ConstIterator< T > end() const noexcept;
    ConstIterator< T > cend() const noexcept;

    T& front() noexcept;
    T& back() noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void assign(size_t n, const T& value);
    template< typename InputIterator >
    void assign(InputIterator first, InputIterator last);
    void assign(std::initializer_list< T > il);
    void push_front(const T& value);
    void pop_front() noexcept;
    void push_back(const T& value);
    void pop_back() noexcept;
    Iterator< T > insert(Iterator< T > position, const T& value);
    Iterator< T > insert(Iterator< T > position, T&& value);
    Iterator< T > insert(Iterator< T > position, size_t n, const T& value);
    template< typename InputIterator >
    Iterator< T > insert(Iterator< T > position, InputIterator first, InputIterator last);
    Iterator< T > insert(Iterator< T > position, std::initializer_list< T > il);
    Iterator< T > erase(Iterator< T > position);
    Iterator< T > erase(Iterator< T > first, Iterator< T > last);
    template< typename... Args >
    Iterator< T > emplace_front(Args&&... args);
    template< typename... Args >
    Iterator< T > emplace_back(Args&&... args);
    template< typename... Args >
    Iterator< T > emplace(Iterator< T > position, Args&&... args);
    void swap(List< T >& other) noexcept;
    void clear() noexcept;

    void splice(Iterator< T > position, List< T >& other);
    void splice(Iterator< T > position, List< T >& other, Iterator< T > i);
    void splice(Iterator< T > position, List< T >& other, Iterator< T > first, Iterator< T > last);
    void remove(const T& value) noexcept;
    template< typename Predicate >
    void remove_if(Predicate pred);
    void unique();
    template< typename Predicate >
    void unique(Predicate pred);
    void sort();
    template< typename Predicate >
    void sort(Predicate pred);
    void reverse() noexcept;
  private:
    ListNode< T >* fake_;
    ListNode< T >* head_;

    void push_single(ListNode< T >* node);
    ListNode< T >* get_last_node();
    void push_back_value(const T value);
  };

  template< typename T >
  bool operator==(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator!=(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator<(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator<=(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator>(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  bool operator>=(const List< T >& lhs, const List< T >& rhs) noexcept;

  template< typename T >
  struct EqualNode;

  template< typename T >
  bool equal_node(const T& one, const T& two);

  template< typename T >
  bool less_node(const T& one, const T& two);

  template< typename T >
  bool operator==(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    if (lhs.size() != rhs.size())
    {
      return false;
    }
    for (auto it_lhs = lhs.cbegin(), it_rhs = rhs.cbegin(); it_lhs != lhs.cend(); ++it_lhs, ++it_rhs)
    {
      if ((*it_lhs) != (*it_rhs))
      {
        return false;
      }
    }
    return true;
  }

  template< typename T >
  bool operator!=(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (!(lhs == rhs));
  }

  template< typename T >
  bool operator<(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    if (lhs.size() != rhs.size())
    {
      return lhs.size() < rhs.size();
    }
    for (auto it_lhs = lhs.cbegin(), it_rhs = rhs.cbegin(); it_lhs != lhs.cend(); ++it_lhs, ++it_rhs)
    {
      if ((*it_lhs) != (*it_rhs))
      {
        return (*it_lhs) < (*it_rhs);
      }
    }
    return false;
  }

  template< typename T >
  bool operator<=(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (!(rhs < lhs));
  }

  template< typename T >
  bool operator>(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (rhs < lhs);
  }

  template< typename T >
  bool operator>=(const List< T >& lhs, const List< T >& rhs) noexcept
  {
    return (!(lhs < rhs));
  }

  template< typename T >
  List< T >::List():
    fake_(new ListNode< T >{T(), nullptr, nullptr}),
    head_(fake_)
  {}

  template< typename T >
  List< T >::List(const List< T >& other):
    List()
  {
    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
      const T copy = *it;
      push_back_value(copy);
    }
  }

  template< typename T >
  List< T >::List(List< T >&& other) noexcept:
    fake_(std::exchange(other.fake_, nullptr)),
    head_(std::exchange(other.head_, nullptr))
  {}

  template< typename T >
  List< T >::List(size_t n, const T& value):
    List()
  {
    for (size_t i = 0; i < n; i++)
    {
      const T copy = value;
      push_back_value(copy);
    }
  }
  template< typename T >
  template< typename InputIterator >
  List< T >::List(InputIterator first, InputIterator last):
    List()
  {
    while (first != last)
    {
      const T copy = *first;
      push_back_value(copy);
      first++;
    }
  }

  template< typename T >
  List< T >::List(std::initializer_list< T > il):
    List()
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      const T copy = *it;
      push_back_value(copy);
    }
  }

  template< typename T >
  List< T >& List< T >::operator=(const List< T >& other)
  {
    assert(this != std::addressof(other) && "Assigning a list to itself");
    List< T > copy(other);
    swap(copy);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(List< T >&& other) noexcept
  {
    assert(this != std::addressof(other) && "Assigning a list to itself");
    clear();
    fake_ = std::exchange(other.fake_, nullptr);
    head_ = std::exchange(other.head_, nullptr);
    return *this;
  }

  template< typename T >
  List< T >& List< T >::operator=(std::initializer_list< T > il)
  {
    List< T > copy(il);
    swap(copy);
    return *this;
  }

  template< typename T >
  List< T >::~List() noexcept
  {
    clear();
    delete fake_;
  }

  template< typename T >
  Iterator< T > List< T >::begin() noexcept
  {
    return Iterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::begin() const noexcept
  {
    return ConstIterator< T >(head_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cbegin() const noexcept
  {
    return begin();
  }

  template< typename T >
  Iterator< T > List< T >::end() noexcept
  {
    return Iterator< T >(fake_);
  }

  template< typename T >
  ConstIterator< T > List< T >::end() const noexcept
  {
    return ConstIterator< T >(fake_);
  }

  template< typename T >
  ConstIterator< T > List< T >::cend() const noexcept
  {
    return end();
  }

  template< typename T >
  T& List< T >::front() noexcept
  {
    assert(!empty());
    return head_->data;
  }

  template< typename T >
  T& List< T >::back() noexcept
  {
    assert(!empty());
    return fake_->prev->data;
  }

  template< typename T >
  const T& List< T >::front() const noexcept
  {
    assert(!empty());
    return head_->data;
  }

  template< typename T >
  const T& List< T >::back() const noexcept
  {
    assert(!empty());
    return fake_->prev->data;
  }

  template< typename T >
  bool List< T >::empty() const noexcept
  {
    return cbegin() == cend();
  }

  template< typename T >
  size_t List< T >::size() const noexcept
  {
    size_t size = 0;
    auto it = begin();
    while (it != end())
    {
      ++it;
      ++size;
    }
    return size;
  }

  template <typename T >
  void List< T >::assign(size_t n, const T& value)
  {
    clear();
    for (size_t i = 0; i < n; i++)
    {
      const T copy = value;
      push_back_value(copy);
    }
  }

  template< typename T >
  template< typename InputIterator >
  void List< T >::assign(InputIterator first, InputIterator last)
  {
    clear();
    for (; first != last; ++first)
    {
      const T copy = *first;
      push_back_value(copy);
    }
  }

  template< typename T >
  void List< T >::assign(std::initializer_list< T > il)
  {
    clear();
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      const T copy = *it;
      push_back_value(copy);
    }
  }

  template< typename T >
  void List< T >::push_front(const T& value)
  {
    auto node = new ListNode< T >{value, nullptr, nullptr};
    if (empty())
    {
      push_single(node);
    }
    else
    {
      ListNode< T >* subhead = head_;
      head_ = node;
      head_->next = subhead;
      subhead->prev = head_;
    }
  }

  template< typename T >
  void List< T >::pop_front() noexcept
  {
    assert(!empty());
    ListNode< T >* subhead = head_->next;
    delete head_;
    head_ = subhead;
    head_->prev = nullptr;
  }

  template< typename T >
  void List< T >::push_back(const T& value)
  {
    auto node = new ListNode< T >{value, nullptr, nullptr};
    if (empty())
    {
      push_single(node);
    }
    else
    {
      ListNode< T >* subhead = get_last_node();
      subhead->next = node;
      node->prev = subhead;
      node->next = fake_;
      fake_->prev = node;
    }
  }

  template< typename T >
  void List< T >::pop_back() noexcept
  {
    assert(!empty());
    ListNode< T >* subhead = get_last_node();
    if (size() == 1)
    {
      head_ = fake_;
      fake_->prev = nullptr;
      delete subhead;
    }
    else
    {
      subhead->prev->next = fake_;
      fake_->prev = subhead->prev;
      delete subhead;
    }
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, const T& value)
  {
    T copy = value;
    if (position == begin())
    {
      push_front(copy);
    }
    else if(position == end())
    {
      push_back(copy);
    }
    else
    {
      auto node_new = new ListNode< T >{copy, nullptr, nullptr};
      ListNode< T >* node_now = position.node_;
      node_now->prev->next = node_new;
      node_new->prev = node_now->prev;
      node_now->prev = node_new;
      node_new->next = node_now;
    }
    return --position;
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, T&& value)
  {
    if (position == begin())
    {
      push_front(value);
    }
    else if(position == end())
    {
      push_back(value);
    }
    else
    {
      auto node_new = new ListNode< T >{value, nullptr, nullptr};
      ListNode< T >* node_now = position.node_;
      node_now->prev->next = node_new;
      node_new->prev = node_now->prev;
      node_now->prev = node_new;
      node_new->next = node_now;
    }
    return --position;
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, size_t n, const T& value)
  {
    Iterator< T > return_it = insert(position, value);
    for (size_t i = 1; i < n; i++)
    {
      Iterator< T > tmp_it = insert(position, value);
      tmp_it = return_it;
    }
    return return_it;
  }

  template< typename T >
  template< typename InputIterator >
  Iterator< T > List< T >::insert(Iterator< T > position, InputIterator first, InputIterator last)
  {
    Iterator< T > return_it = insert(position, *first);
    ++first;
    while (first != last)
    {
      Iterator< T > tmp_it = insert(position, *first);
      tmp_it = return_it;
      ++first;
    }
    return return_it;
  }

  template< typename T >
  Iterator< T > List< T >::insert(Iterator< T > position, std::initializer_list< T > il)
  {
    auto it = il.begin();
    Iterator< T > return_it = insert(position, *it);
    ++it;
    for(; it != il.end(); ++it)
    {
      Iterator< T > tmp_it = insert(position, *it);
      tmp_it = return_it;
    }
    return return_it;
  }

  template< typename T >
  Iterator< T > List< T >::erase(Iterator< T > position)
  {
    Iterator< T > return_it = ++position;
    --position;
    if (position == begin())
    {
      pop_front();
    }
    else if (position == --end())
    {
      pop_back();
    }
    else
    {
      ListNode< T >* node = position.node_;
      node->prev->next = node->next;
      node->next->prev = node->prev;
      delete node;
    }
    return return_it;
  }

  template< typename T >
  Iterator< T > List< T >::erase(Iterator< T > first, Iterator< T > last)
  {
    while (first != last)
    {
      Iterator< T > next = ++first;
      --first;
      Iterator < T > tmp_it = erase(first);
      tmp_it = last;
      first = next;
    }
    return last;
  }
  template< typename T >
  template< typename... Args >
  Iterator< T > List< T >::emplace_front(Args&&... args)
  {
    return insert(begin(), T{args...});
  }

  template< typename T >
  template< typename... Args >
  Iterator< T > List< T >::emplace_back(Args&&... args)
  {
    return insert(end(), T{args...});
  }

  template< typename T >
  template< typename... Args >
  Iterator< T > List< T >::emplace(Iterator< T > position, Args&&... args)
  {
    return insert(position, T{args...});
  }

  template< typename T >
  void List< T >::swap(List< T >& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(head_, other.head_);
  }

  template< typename T >
  void List< T >::clear() noexcept
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template< typename T >
  void List< T >::splice(Iterator< T > position, List< T >& other)
  {
    assert(*this != other && "Splicing a list to itself");
    auto other_it = other.begin();
    while (other_it != other.end())
    {
      Iterator< T > other_it_next = ++other_it;
      --other_it;
      splice(position, other, other_it);
      other_it = other_it_next;
    }
  }
  template< typename T >
  void List< T >::splice(Iterator< T > position, List< T >& other, Iterator< T > other_it)
  {
    assert(position.node_ != nullptr && "Iterator is not valid");
    assert(other_it.node_ != nullptr && "Iterator is not valid");
    assert(other_it != other.end() && "Iterator is not valid");

    ListNode< T >* node_now = position.node_;
    ListNode< T >* other_node_now = other_it.node_;
    if (position == begin())
    {
      head_ = other_node_now;
    }
    else
    {
      node_now->prev->next = other_node_now;
    }
    if (other_it == other.begin())
    {
      other.head_ = other_node_now->next;
    }
    else
    {
      other_node_now->prev->next = other_node_now->next;
    }
    other_node_now->next->prev = other_node_now->prev;
    other_node_now->prev = node_now->prev;
    other_node_now->next = node_now;
    node_now->prev = other_node_now;
  }
  template< typename T >
  void List< T >::splice(Iterator< T > position, List< T >& other, Iterator< T > first, Iterator< T > last)
  {
    auto other_it = first;
    while (other_it != last)
    {
      Iterator< T > other_it_next = ++other_it;
      --other_it;
      splice(position, other, other_it);
      other_it = other_it_next;
    }
  }
  template< typename T >
  void List < T >::remove(const T& value) noexcept
  {
    auto pred = EqualNode< T >{value};
    remove_if< EqualNode< T > >(pred);
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::remove_if(Predicate pred)
  {
    assert(!empty());
    ListNode< T >* subhead = head_;
    auto it = begin();
    while (it != end())
    {
      Iterator< T > it_next = ++it;
      --it;
      ListNode< T >* subnext = subhead->next;
      if (pred(*it))
      {
        if (it == begin())
        {
          pop_front();
        }
        else if (it == --end())
        {
          pop_back();
        }
        else
        {
          subhead->prev->next = subhead->next;
          subhead->next->prev = subhead->prev;
          delete subhead;
        }
      }
      subhead = subnext;
      it = it_next;
    }
  }

  template< typename T >
  void List< T >::unique()
  {
    unique(equal_node< T >);
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::unique(Predicate pred)
  {
    ListNode< T >* subhead = head_;
    auto it = begin();
    ++it;
    while (it != end() && it.node_ != nullptr)
    {
      Iterator< T > it_next = ++it;
      --it;
      if (pred(*it, subhead->data))
      {
        erase(it);
      }
      else
      {
        subhead = subhead->next;
      }
      it = it_next;
    }
  }

  template< typename T >
  void List< T >::sort()
  {
    sort(less_node< T >);
  }

  template< typename T >
  template< typename Predicate >
  void List< T >::sort(Predicate pred)
  {
    for (auto it1 = begin(); it1 != end(); ++it1)
    {
      auto min_it = it1;
      auto it2 = ++it1;
      --it1;
      for (; it2 != end(); ++it2)
      {
        if (pred(*it2, *min_it))
        {
          min_it = it2;
        }
      }
      std::swap(*it1, *min_it);
    }
  }

  template< typename T >
  void List< T >::reverse() noexcept
  {
    if (size() < 2)
    {
      return;
    }
    auto it = ++begin();
    std::swap(head_->next, head_->prev);
    head_->next = fake_;
    while (it != --end())
    {
      auto it_next = ++it;
      --it;
      ListNode< T >* node = it.node_;
      std::swap(node->next, node->prev);
      it = it_next;
    }
    ListNode< T >* tail = it.node_;
    std::swap(tail->next, tail->prev);
    tail->prev = nullptr;
    fake_->prev = head_;
    head_ = tail;
  }

  template< typename T >
  void List< T >::push_single(ListNode< T >* node)
  {
    head_ = node;
    head_->next = fake_;
    fake_->prev = head_;
  }

  template< typename T >
  ListNode< T >* List< T >::get_last_node()
  {
    ListNode< T >* subhead = head_;
    for (auto it = ++begin(); it != end(); ++it)
    {
      subhead = subhead->next;
    }
    return subhead;
  }

  template< typename T >
  bool is_equal_node(const T& value, const T& data)
  {
    return data == value;
  }

  template< typename T >
  void List< T >::push_back_value(const T value)
  {
    auto node = new ListNode< T >{value, nullptr, nullptr};
    if (empty())
    {
      push_single(node);
    }
    else
    {
      ListNode< T >* subhead = get_last_node();
      subhead->next = node;
      node->prev = subhead;
      node->next = fake_;
      fake_->prev = node;
    }
  }
}
#endif
