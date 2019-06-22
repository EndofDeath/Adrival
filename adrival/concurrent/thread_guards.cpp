
#include "thread_guards.h"

namespace adrival{

class thread_guards::impl
{
public:
	impl(unsigned int count):count_(count),threads_(new std::thread[count]){}
	~impl()
	{
		if (threads_)
		{
			for (auto& t : *this)
			{
				if (t.joinable())
					t.join();
			}
			delete[] threads_;
		}
		threads_ = nullptr;
	}
	std::thread* begin() const
	{
		return threads_;
	}
	std::thread* end() const
	{
		return threads_ + count_;
	}
private:
	const unsigned int count_;
	std::thread* threads_;
};

thread_guards::thread_guards()
	:pimp_(nullptr)
{
}

thread_guards::~thread_guards()
{
	if (pimp_)
		delete pimp_;
	pimp_ = nullptr;
}

thread_guards::thread_guards(thread_guards && other) noexcept
{
	pimp_ = other.pimp_;
	other.pimp_ = nullptr;
}

thread_guards & thread_guards::operator=(thread_guards && other) noexcept
{
	if (this != &other)
	{
		if (pimp_)
			delete pimp_;
		pimp_ = other.pimp_;
		other.pimp_ = nullptr;
	}
	return *this;
}

bool thread_guards::create(unsigned int thread_count)
{
	if(0 == thread_count)
		return false;
	if (pimp_)
		delete pimp_;
	pimp_ = new impl(thread_count);
	return true;
}

std::thread * thread_guards::begin() const
{
	if(nullptr == pimp_)
		return nullptr;
	return pimp_->begin();
}

std::thread * thread_guards::end() const
{
	if (nullptr == pimp_)
		return nullptr;
	return pimp_->end();
}

}