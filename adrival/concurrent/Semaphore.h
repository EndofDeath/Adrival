#ifndef ADRIVAL_SEMAPHORE_H
#define ADRIVAL_SEMAPHORE_H
#include <chrono>

namespace adrival{

class semaphore
{
public:
	semaphore(long initcount, long maxcount);
	~semaphore();

	void wait();

	bool wait_for(const std::chrono::milliseconds& timeout);

	void release(long count);
private:
	semaphore(const semaphore&) = delete;
	semaphore(semaphore&&) = delete;
	semaphore& operator=(const semaphore&) = delete;
	semaphore& operator=(semaphore&&) = delete;

	class impl;
	impl* pimp_;
};

}
#endif // !ADRIVAL_SEMAPHORE_H