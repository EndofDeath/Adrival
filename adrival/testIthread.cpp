
#include <iostream>
#include <atomic>
#include "concurrent/Ithread.h"

using namespace std::literals;

class threada :public adrival::Ithread
{
public:
	threada()
	{}
	~threada()
	{}
	void stop()
	{
		done = false;
	}
private:
	void run()
	{
		do
		{
			std::cout << "threada run " << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(100ms);
		} while (done);
	}
	std::atomic_bool done{ true };
};

class threadb :public adrival::Ithread
{
public:
	threadb()
	{}
	~threadb()
	{}
	void settask(std::function<void()> task)
	{
		fun = task;
	}
	void stop()
	{
		done = false;
	}
private:
	void run()
	{
		do
		{
			if (fun) fun();
			std::this_thread::sleep_for(100ms);
		} while (done);
	}
	std::atomic_bool done{ true };
	std::function<void()> fun;
};

void test_thread()
{
	threada a;
	a.start();
	std::this_thread::sleep_for(1000ms);
	a.stop();
	a.join();

	threadb b;
	b.settask([] {
		std::cout << 1 << std::endl;
	});
	b.start();
	std::this_thread::sleep_for(1000ms);
	b.settask([] {
		std::cout << 2 << std::endl;
	});
	std::this_thread::sleep_for(1000ms);
	b.settask([] {
		std::cout << 3 << std::endl;
	});
	std::this_thread::sleep_for(1000ms);
	b.stop();
	b.join();
}