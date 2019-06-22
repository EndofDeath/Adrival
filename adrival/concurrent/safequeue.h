#ifndef ADRIVAL_SAFE_QUEUE_H
#define ADRIVAL_SAFE_QUEUE_H

#include <memory>
#include <mutex>
namespace adrival {

template<typename T>
class safequeue
{
protected:
	using autolock = std::lock_guard<std::mutex>;
	using movelock = std::unique_lock<std::mutex>;
	using value_ptr = std::shared_ptr<T>;
	struct node;
	using element = std::unique_ptr<node>;
public:
	safequeue() : head_(new node), tail_(head_.get()) {}	
	safequeue(const safequeue& other) = delete;
	safequeue& operator=(const safequeue& other) = delete;

	void push(T&& new_value)
	{
		value_ptr new_data(std::make_shared<T>(std::move(new_value)));
		push_back(new_data);
	}
	void push(const T& new_value)
	{
		value_ptr new_data(std::make_shared<T>(new_value));
		push_back(new_data);
	}
	value_ptr wait_and_pop()
	{
		element const old_head = wait_pop_front();
		return old_head->data;	
	}
	void wait_and_pop(T& value)
	{
		element const old_head = wait_pop_front(value);
	}
	value_ptr try_pop()
	{
		element old_head = try_pop_front();
		return old_head ? old_head->data : value_ptr();
	}
	bool try_pop(T& value)
	{
		element const old_head = try_pop_front(value);
		return (bool)old_head;
	}
	bool empty()
	{
		autolock head_lock(head_mutex_);
		return head_.get() == get_tail();
	}
protected:
	struct node
	{
		value_ptr data;
		element next;
		node* prev{};
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
	void push_back(value_ptr& new_data)
	{
		element p(new node);
		{
			autolock tail_lock(tail_mutex_);
			tail_->data = new_data;
			node* new_tail = p.get();
			tail_->next = std::move(p);
			new_tail->prev = tail_;
			tail_ = new_tail;
		}
		cond_.notify_one();
	}
	element pop_front()
	{
		element old_head = std::move(head_);
		head_ = std::move(old_head->next);
		head_.get()->prev = nullptr;
		return old_head;
	}
	movelock wait_for_data()
	{
		movelock head_lock(head_mutex_);
		cond_.wait(head_lock, [this] {return head_.get() != get_tail(); });
		return std::move(head_lock);//返回unique_lock，以便下一个函数接手。保证函数交替期间锁不会断	
	}
	element wait_pop_front()
	{
		movelock head_lock(wait_for_data());
		return pop_front();
	}
	element wait_pop_front(T& value)
	{
		movelock head_lock(wait_for_data());
		value = std::move(*head_->data);//	
		return pop_front();
	}
	element try_pop_front()
	{
		autolock head_lock(head_mutex_);
		if (head_.get() == get_tail())
			return element();
		return pop_front();
	}
	element try_pop_front(T& value)
	{
		autolock head_lock(head_mutex_);
		if (head_.get() == get_tail())
			return element();
		value = std::move(*head_->data);
		return pop_front();
	}
};

}
#endif // !ADRIVAL_SAFE_QUEUE_H