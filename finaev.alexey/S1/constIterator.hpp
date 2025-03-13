#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP
#include <memory>
#include <cassert>
#include "node.hpp"
#include "iterator.hpp"


namespace finaev
{
	template < class T >
	struct constListIterator
	{
	public:
		using this_t = constListIterator< T >;
		constListIterator();
		explicit constListIterator(const Node<T>* node);
		~constListIterator() = default;
		const T& operator*() const;
		const T* operator->() const;
		this_t& operator++();
		this_t operator++(int);
		bool operator==(const constListIterator< T >& i) const;
		bool operator!=(const constListIterator< T >& i) const;
	private:
		const Node< T >* node_;
	};

	template < class T >
	constListIterator< T >::constListIterator() :
		node_(nullptr)
	{}

	template < class T >
	constListIterator< T >::constListIterator(const Node<T>* node) :
		node_(node)
	{}

	template < class T >
	const T&  constListIterator< T >::operator*() const
	{
		assert(node_ != nullptr);
		return node_->data;
	}

	template < class T >
	const T* constListIterator< T >::operator->() const
	{
		assert(node_ != nullptr);
		return std::addressof(node_->data);
	}

	template < class T >
	constListIterator< T >& constListIterator< T >::operator++()
	{
		assert(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}

	template < class T >
	constListIterator< T > constListIterator< T >::operator++(int)
	{
		assert(node_ != nullptr);
		constListIterator< T > res(*this);
		++(*this);
		return res;
	}

	template < class T >
	bool constListIterator< T >::operator==(const constListIterator< T >& i) const
	{
		return node_ == i.node_;
	}

	template < class T >
	bool constListIterator< T >::operator!=(const constListIterator< T >& i) const
	{
		return !(node_ == i.node_);
	}

}
#endif
