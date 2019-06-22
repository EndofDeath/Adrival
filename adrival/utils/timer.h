#ifndef ADRIVAL_TIMER_H
#define ADRIVAL_TIMER_H

#include <chrono>

namespace adrival {

class Timer
{
	using system_clock = std::chrono::high_resolution_clock;
	using time_point = std::chrono::time_point < system_clock >;

public:
	Timer() :start_(system_clock::now()) {}
	~Timer() = default;

	void Reset() { start_ = system_clock::now(); }

	double elapsed() const
	{
		return std::chrono::duration_cast<std::chrono::duration<double>>(system_clock::now() - start_).count();
	}

	auto elapsedmicrosecond() const
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(system_clock::now() - start_).count();
	}

	auto elapsedmillisecond() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now() - start_).count();
	}

	auto elapsedsecond() const
	{
		return std::chrono::duration_cast<std::chrono::seconds>(system_clock::now() - start_).count();
	}

private:
	time_point start_;
};

}
#endif // !ADRIVAL_TIMER_H