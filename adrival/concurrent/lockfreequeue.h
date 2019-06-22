#ifndef ADRIVAL_LOCKFREE_QUEUE_H
#define ADRIVAL_LOCKFREE_QUEUE_H

#include <atomic>
#include "../utility/nocopyable.h"

namespace adrival {

template<typename T>
class lockfreequeue:public nocopyable
{

public:
	lockfreequeue() : head_(new node{}), tail_(head_.load()) {}
	~lockfreequeue()
	{
		node* free = head_;
		node* nextfree;
		do
		{
			nextfree = free->next;
			delete free;
			free = nextfree;
		} while (nullptr != free);
	}

	void push(T&& new_value)
	{
		node* newnode = new node{};
		node* empty{ nullptr };
		node* oldtail;
		do
		{
			oldtail = tail_;
		} while (!oldtail->next.compare_exchange_weak(empty, newnode));
		oldtail->data = new_value;
		tail_.compare_exchange_weak(oldtail, newnode);
	}
	void push(const T& new_value)
	{
		node* newnode = new node{};
		node* empty{ nullptr };
		node* oldtail;
		do
		{
			oldtail = tail_;
		} while (!oldtail->next.compare_exchange_weak(empty,newnode));
		oldtail->data = new_value;
		tail_.compare_exchange_weak(oldtail, newnode);
	}
	bool try_pop(T& value)
	{
		if (empty()) return false;
		node* oldhead;
		do
		{
			oldhead = head_;
		} while (!head_.compare_exchange_weak(oldhead, oldhead->next));
		value = oldhead->data;
		delete oldhead;
		return true;
	}
	bool empty()
	{
		return head_ == tail_;
	}
protected:
	struct node
	{
		T data;
		std::atomic<node*> next;
	};

	using atomic_ptr = std::atomic<node*>;

	atomic_ptr head_;
	atomic_ptr tail_;
};

}
#endif // !ADRIVAL_LOCKFREE_QUEUE_H