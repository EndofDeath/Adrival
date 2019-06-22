#ifndef ADRIVAL_RING_QUEUE_H
#define ADRIVAL_RING_QUEUE_H

#include <atomic>

namespace adrival {

	template<class T>
	class ringqueue
	{
	public:
		ringqueue()
			:buffer_(new T[size] {})
			, flags_(new std::atomic_bool[size]{})
			, head_index_(0)
			, tail_index_(0)
		{}
		~ringqueue()
		{
			delete[] buffer_;
			delete[] flags_;
		}
		bool try_push(const T& value)
		{
			size_t index{ 0 };
			bool empty{ false };
			do
			{
				index = tail_index_;
				if (size == (index - head_index_)) return false;
			} while (!flags_[index&(size - 1)].compare_exchange_weak(empty,true));
			buffer_[index&(size - 1)] = value;
			tail_index_.compare_exchange_weak(index, index + 1);
			return true;
		}
		bool try_pop(T& value)
		{
			size_t index{ 0 };
			do
			{
				index = head_index_;
				if (index == tail_index_) return false;
			} while (!head_index_.compare_exchange_weak(index,index+1));
			value = std::move(buffer_[index&(size - 1)]);
			flags_[index&(size - 1)] = false;
			return true;
		}
	private:
		static const size_t size{ 256 };
		T* buffer_;
		std::atomic_bool * flags_;
		std::atomic<size_t> head_index_;
		std::atomic<size_t> tail_index_;
	};

}
#endif // !ADRIVAL_RING_QUEUE_H