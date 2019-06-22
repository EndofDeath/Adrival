#include "semaphore.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace adrival {

class semaphore::impl
{
public:
	impl(long initcount, long maxcount)
		:_currentcount(initcount)
		, _maxcount(maxcount)
	{}
	std::mutex mutex_;
	std::condition_variable con_;
	std::atomic_long _currentcount;
	const long _maxcount;
};

semaphore::semaphore(long initcount, long maxcount)
	:pimp_(new impl(initcount,maxcount))
{
}

semaphore::~semaphore()
{
	delete pimp_;
}

void semaphore::wait()
{
	std::unique_lock<std::mutex> _lock(pimp_->mutex_);
	pimp_->con_.wait(_lock, [this] {return 0 < pimp_->_currentcount; });
	pimp_->_currentcount--;
}

bool semaphore::wait_for(const std::chrono::milliseconds& timeout)
{
	std::unique_lock<std::mutex> _lock(pimp_->mutex_);
	if (pimp_->con_.wait_for(_lock, timeout, [this] {return 0 < pimp_->_currentcount; }))
	{
		pimp_->_currentcount--;
		return true;
	}
	return false;
}

void semaphore::release(long count)
{
	if (0 > count)
		return;
	std::lock_guard<std::mutex> lock(pimp_->mutex_);
	if (pimp_->_maxcount == pimp_->_currentcount || pimp_->_maxcount < pimp_->_currentcount + count)
		count = pimp_->_maxcount - pimp_->_currentcount;
	pimp_->_currentcount += count;
	while (count--)
	{
		pimp_->con_.notify_one();
	}
}

}