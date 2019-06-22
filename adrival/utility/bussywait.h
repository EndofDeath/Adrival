#ifndef ARIVAL_BUSSYWAIT_H
#define ARIVAL_BUSSYWAIT_H

#include <thread>
#include <chrono>

namespace adrival {

	template <class _pred>
	inline void bussywait(_pred pred)
	{
		do
		{
			if (pred()) break;
			std::this_thread::yield();
		} while (true);
	}

	template <class _pred>
	inline bool bussywait_for(_pred pred, const std::chrono::milliseconds& timeout)
	{
		auto begin = std::chrono::high_resolution_clock::now();
		auto end = begin + timeout;
		do
		{
			if (pred()) return true;
			std::this_thread::yield();
		} while (std::chrono::high_resolution_clock::now() < end);
		return false;
	}

}
#endif // !ARIVAL_BUSSYWAIT_H