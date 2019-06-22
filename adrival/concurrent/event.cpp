#include "event.h"
#include <mutex>
#include <condition_variable>
#include "../utility/nocopyable.h"
#include "../utility/fixedringbuffer.h"
#include "../utility/bussywait.h"

namespace adrival {

class Event::impl :public nocopyable
{
public:
	impl(bool autoreset) :ready_(false), autoreset_(autoreset) {}

	std::mutex mutex_;
	std::condition_variable con_;
	bool ready_;
	const bool autoreset_;
};

Event::Event(bool autoreset)
	: pimp_(new impl(autoreset))
{
}

Event::~Event()
{
	delete pimp_;
}

void Event::wait()
{
	std::unique_lock<std::mutex> lock(pimp_->mutex_);
	pimp_->con_.wait(lock, [this] {return true == pimp_->ready_; });
	if (pimp_->autoreset_) pimp_->ready_ = false;
}

bool Event::wait_for(const std::chrono::milliseconds& timeout)
{
	std::unique_lock<std::mutex> lock(pimp_->mutex_);
	if (pimp_->con_.wait_for(lock, timeout, [this] {return true == pimp_->ready_; }))
	{
		if (pimp_->autoreset_) pimp_->ready_ = false;
		return true;
	}
	return false;
}

void Event::set()
{
	std::lock_guard<std::mutex> lock(pimp_->mutex_);
	pimp_->ready_ = true;
	pimp_->con_.notify_all();
}

void Event::reset()
{
	std::lock_guard<std::mutex> lock(pimp_->mutex_);
	pimp_->ready_ = false;
}

bool Event::ready() const
{
	std::lock_guard<std::mutex> lock(pimp_->mutex_);
	return pimp_->ready_;
}

class Event::multiple::inner
{
public:
	inner(Event* events, int count)
		:events_(events), count_(count), ready_ids_(count) {}
	inner(const inner& other)
		:events_(other.events_)
		, count_(other.count_)
		, ready_ids_(other.count_)
	{
	}
	Event* events_{};
	int count_{};
	fixedringbuffer<int> ready_ids_;
};

Event::multiple::multiple(Event * events, int count)
	:pimp_(new inner(events, count))
{
}

Event::multiple::~multiple()
{
	if (pimp_)
		delete pimp_;
	pimp_ = nullptr;
}

Event::multiple::multiple(const multiple & other)
	:pimp_(new inner(*other.pimp_))
{
}

Event::multiple::multiple(multiple && other) noexcept
	:pimp_(other.pimp_)
{
	other.pimp_ = nullptr;
}

Event::multiple & Event::multiple::operator=(const multiple & other)
{
	if (this != &other)
		pimp_ = new inner(*other.pimp_);
	return *this;
}

Event::multiple & Event::multiple::operator=(multiple && other) noexcept
{
	pimp_ = other.pimp_;
	other.pimp_ = nullptr;
	return *this;
}

int Event::multiple::wait()
{
	int id = 0;
	bussywait([this] {
		if (pimp_->ready_ids_.empty())
		{
			for (int id = 0; id < pimp_->count_; id++)
			{
				std::lock_guard<std::mutex> lock(pimp_->events_[id].pimp_->mutex_);
				if (pimp_->events_[id].pimp_->ready_)
					pimp_->ready_ids_.push(id);
				if (pimp_->events_[id].pimp_->autoreset_)
					pimp_->events_[id].pimp_->ready_ = false;
			}
		}
		return !pimp_->ready_ids_.empty();
	});
	pimp_->ready_ids_.pop(id);
	return id + 1;
}

int Event::multiple::wait_for(const std::chrono::milliseconds & timeout)
{
	int id = 0;
	if (bussywait_for([this] {
		if (pimp_->ready_ids_.empty())
		{
			for (int id = 0; id < pimp_->count_; id++)
			{
				std::lock_guard<std::mutex> lock(pimp_->events_[id].pimp_->mutex_);
				if (pimp_->events_[id].pimp_->ready_)
					pimp_->ready_ids_.push(id);
				if (pimp_->events_[id].pimp_->autoreset_)
					pimp_->events_[id].pimp_->ready_ = false;
			}
		}
		return !pimp_->ready_ids_.empty();
	}, timeout))
	{
		pimp_->ready_ids_.pop(id);
		return id + 1;
	}
	return 0;
}

}