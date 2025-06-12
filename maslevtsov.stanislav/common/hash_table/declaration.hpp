#ifndef HASH_TABLE_DECLARATION_HPP
#define HASH_TABLE_DECLARATION_HPP

#include <functional>
#include <initializer_list>
#include <vector/definition.hpp>
#include "hash_table_iterator.hpp"

namespace detail {
  enum class SlotState
  {
    EMPTY,
    OCCUPIED,
    DELETED,
  };

  template< class T >
  struct Slot
  {
    T data;
    SlotState state = SlotState::EMPTY;
  };
}

namespace maslevtsov {
  template< class Key, class T, class Hash = std::hash< Key >, class KeyEqual = std::equal_to< Key > >
  class HashTable
  {
  public:
    using value_type = std::pair< Key, T >;
    using size_type = std::size_t;
    using iterator = HashTableIterator< value_type, detail::HashTableIteratorType::NONCONSTANT >;
    using const_iterator = HashTableIterator< value_type, detail::HashTableIteratorType::CONSTANT >;

    HashTable() noexcept;
    HashTable(const HashTable& rhs);
    HashTable(HashTable&& rhs) noexcept;
    template< class InputIt >
    HashTable(InputIt first, InputIt last);
    HashTable(std::initializer_list< value_type > ilist);
    ~HashTable() noexcept;

    HashTable& operator=(const HashTable& rhs);
    HashTable& operator=(HashTable&& rhs) noexcept;
    HashTable& operator=(std::initializer_list< value_type > ilist);

    T& operator[](const Key& key);
    T& at(const Key& key);
    const T& at(const Key& key) const;
    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_type size() const noexcept;

    void clear() noexcept;
    std::pair< iterator, bool > insert(const value_type& value);
    iterator insert(const_iterator hint, const value_type& value);
    template< class InputIt >
    void insert(InputIt first, InputIt last);
    template< class... Args >
    std::pair< iterator, bool > emplace(Args&&... args);
    template< class... Args >
    iterator emplace_hint(const_iterator hint, Args&&... args);
    iterator erase(iterator pos);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(const Key& key);
    void swap(HashTable& other);

    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float ml);
    void rehash(size_type count);

  private:
    Vector< detail::Slot< value_type > > slots_;
    size_type size_;
    Hash hash_;
    KeyEqual key_equal_;
    float max_load_factor_ = 1.0;
  };
}

#endif
