#ifndef ADRIVAL_SAFECOUNTER_H
#define ADRIVAL_SAFECOUNTER_H

#include <shared_mutex>

namespace adrival {

	using readlock = std::shared_lock<std::shared_mutex>;
	using writelock = std::unique_lock<std::shared_mutex>;

	class safecounter
	{
	public:
		safecounter() = default;
		
		unsigned int get() const {
			readlock lock(mutex_);
			return value_;
		}
		
		void increment() {
			writelock lock(mutex_);
			value_++;
		}

		void decrement() {
			writelock lock(mutex_);
			value_--;
		}
		
		void reset(unsigned int value=0) {
			writelock lock(mutex_);
			value_ = value;
		}
	private:
		mutable std::shared_mutex mutex_;
		unsigned int value_ = 0;
	};

}
#endif // !ADRIVAL_SAFECOUNTER_H