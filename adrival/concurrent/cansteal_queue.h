#ifndef ADRIVAL_CANSTEAL_QUEUE_H
#define ADRIVAL_CANSTEAL_QUEUE_H

#include "safequeue.h"

namespace adrival {

template<typename T>
class cansteal_queue:public safequeue<T>
{
public:
	value_ptr try_steal()
	{
		element back = try_pop_back();
		return back ? back->data : value_ptr();
	}
	bool try_steal(T& value)
	{
		element const back = try_pop_back(value);
		return (bool)back;
	}
private:
	element pop_back()
	{
		element back = std::move(tail_->prev->prev->next);
		tail_->prev->prev->next = std::move(tail_->prev->next);
		tail_->prev = tail_->prev->prev;
		return back;
	}
	element try_pop_back()
	{
		autolock head_lock(head_mutex_);
		if (head_.get() == get_tail() || head_.get() == get_tail()->prev)
			return element();
		autolock tail_lock(tail_mutex_);
		return pop_back();
	}
	element try_pop_back(T& value)
	{
		autolock head_lock(head_mutex_);
		if (head_.get() == get_tail() || head_.get() == get_tail()->prev)
			return element();
		autolock tail_lock(tail_mutex_);
		value = std::move(*tail_->prev->data);
		return pop_back();
	}
};

}
#endif // !ADRIVAL_CANSTEAL_QUEUE_H