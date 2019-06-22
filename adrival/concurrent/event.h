#ifndef ADRIVAL_EVENT_H
#define ADRIVAL_EVENT_H

#include <chrono>

namespace adrival{

class Event
{
public:
	Event(bool autoreset = true);
	~Event();

	void wait();

	bool wait_for(const std::chrono::milliseconds& timeout);

	void set();

	void reset();

	bool ready() const;

	class multiple
	{
	public:
		multiple(Event* events, int count);
		~multiple();
		multiple(const multiple& other);
		multiple(multiple&& other) noexcept;
		multiple& operator=(const multiple& other);
		multiple& operator=(multiple&& other) noexcept;
		int wait();
		int wait_for(const std::chrono::milliseconds& timeout);
	private:
		class inner;
		inner* pimp_;
	};
	friend multiple;
private:
	Event(const Event&) = delete;
	Event(Event&&) = delete;
	Event& operator=(const Event&) = delete;
	Event& operator=(Event&&) = delete;

	class impl;
	impl* pimp_;
};

using multiple_event = Event::multiple;

}
#endif //!ADRIVAL_EVENT_H