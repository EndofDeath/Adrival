#ifndef ADRIVAL_BLOCK_SAFE_QUEUE_H
#define ADRIVAL_BLOCK_SAFE_QUEUE_H

#include <memory>
#include <mutex>
#include <chrono>

namespace adrival{

template<typename T>
class blocksafequeue 
{
	using autolock = std::lock_guard<std::mutex>;
	using movelock = std::unique_lock<std::mutex>;
	struct node;
	using element = std::unique_ptr<node>;
public:
	blocksafequeue() : head_(new node{}), tail_(head_.get()) {}
	blocksafequeue(const blocksafequeue& other) = delete;
	blocksafequeue& operator=(const blocksafequeue& other) = delete;

	void push(const T& value)
	{
		auto newtail = new node{};
		{
			autolock tail_lock(tail_mutex_);
			tail_->data = value;
			tail_->next.reset(newtail);
			tail_ = newtail;
		}
		cond_.notify_one();
	}
	void push(T&& value)
	{
		auto newtail = new node{};
		{
			autolock tail_lock(tail_mutex_);
			tail_->data = value;
			tail_->next.reset(newtail);
			tail_ = newtail;
		}
		cond_.notify_one();
	}
	void push_bulk(T* values, size_t count)
	{
		if (nullptr == values || 0 == count) return;
		node* newhead = new node{};
		node* newtail = newhead;
		for (size_t i = 1; i < count; i++)
		{
			newtail->data = values[i];
			newtail->next.reset(new node{});
			newtail = newtail->next.get();
		}
		{
			autolock tail_lock(tail_mutex_);
			tail_->data = values[0];
			tail_->next.reset(newhead);
			tail_ = newtail;
		}
		cond_.notify_one();
	}
	template <size_t N>
	void push_bulk(T(&values)[N])
	{
		push_bulk(values, N);
	}
	//Blocks the current thread until it's not empty.
	void wait_pop(T& value)
	{
		element oldhead;
		{
			movelock head_lock(head_mutex_);
			cond_.wait(head_lock, [this] {return head_.get() != get_tail(); });
			oldhead = pop_head();
		}
		value = oldhead->data;
	}
	//Returns false without setting `value` if the timeout expires, otherwise assigns to `value` and returns true.
	bool wait_pop_for(T& value, const std::chrono::milliseconds& timeout)
	{
		element oldhead;
		{
			movelock head_lock(head_mutex_);
			if (!cond_.wait_for(head_lock, timeout, [this] {return head_.get() != get_tail(); }))
				return false;
			auto oldhead = pop_head();
		}
		value = oldhead->data;
		return true;
	}
	//Returns false without setting `value` if it's empty, otherwise assigns to `value` and returns true.
	bool try_pop(T& value)
	{
		element oldhead;
		{
			autolock head_lock(head_mutex_);
			if (head_.get() == get_tail()) return false;
			oldhead = pop_head();
		}
		value = oldhead->data;
		return true;
	}
	bool empty()
	{
		autolock head_lock(head_mutex_);
		return (head_.get() == get_tail());
	}
private:	
	struct node {
		T data;
		element next;
	};
	std::mutex head_mutex_;
	element head_;
	std::mutex tail_mutex_;
	node* tail_;
	std::condition_variable cond_;

	node* get_tail()
	{
		autolock tail_lock(tail_mutex_);
		return tail_;	
	}
	element pop_head()
	{
		element old_head = std::move(head_);	
		head_ = std::move(old_head->next);
		return old_head;
	}

};

}
#endif // !ADRIVAL_BLOCK_SAFE_QUEUE_H