#ifndef ADRIVAL_FIXED_RINGBUFFER_H
#define ADRIVAL_FIXED_RINGBUFFER_H

#include "../utility/nocopyable.h"
namespace adrival{

template<class T>
class fixedringbuffer:public nocopyable
{
public:
	fixedringbuffer(size_t size)
		:size_(size<4?4:size)
		,buffer_(new T[size_]{})
	{
	}
	~fixedringbuffer()
	{
		delete[] buffer_;
	}
	bool push(const T& value)
	{
		if (size_ == count_) return false;
		buffer_[end_index_] = value;
		end_index_ = (end_index_ + 1) % size_;
		count_++;
		return true;
	}
	bool pop(T& value)
	{
		if (0 == count_) return false;
		value = buffer_[begin_index_];
		begin_index_ = (begin_index_ + 1) % size_;
		count_--;
		return true;
	}
	bool empty() const
	{
		return 0 == count_;
	}
	bool full() const
	{
		return size_ == count_;
	}
private:
	const size_t size_;
	T* buffer_;
	size_t count_{ 0 };
	size_t begin_index_{ 0 };
	size_t end_index_{ 0 };
};

}
#endif // !ADRIVAL_FIXED_RINGBUFFER_H