#ifndef ADRIVAL_THREAD_GUARDS_H
#define ADRIVAL_THREAD_GUARDS_H

#include <thread>
namespace adrival{

class thread_guards
{
public:
	thread_guards();
	~thread_guards();
	thread_guards(const thread_guards&) = delete;
	thread_guards& operator=(const thread_guards&) = delete;
	thread_guards(thread_guards&& other) noexcept;
	thread_guards& operator=(thread_guards&& other) noexcept;
	bool create(unsigned int thread_count);
	std::thread* begin() const;
	std::thread* end() const;
private:
	class impl;
	impl* pimp_;
};

}
#endif // !ADRIVAL_THREAD_GUARDS_H