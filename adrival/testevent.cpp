#include <iostream>
#include <future>
#include "concurrent/event.h"

static constexpr auto numbermax = 8;
thread_local static auto icount = 10;
using namespace std::literals;

void test_event() 
{
	adrival::Event e[numbermax];
	std::future<void> fi[numbermax];
	for (int i = 0; i < numbermax; i++)
	{
		fi[i] = std::async([&e](int id) {
			while (icount)
			{
				e[id - 1].wait();
				std::cout << id << R"(   thread id:)" << std::this_thread::get_id() << std::endl;
				icount--;
				e[id == numbermax ? 0 : id].set();
			}
		}, i + 1);
	}
	e[0].set();
	for (auto& f : fi)
		f.wait();
}

void test_multiplevent()
{
	adrival::Event e[3]{ false };
	auto f = std::async(std::launch::deferred, [&e] {
		adrival::multiple_event me(e, 3);
		while (true)
		{
			int id = me.wait_for(10ms);
			std::cout << "event id:" << id << std::endl;
			if (0 == id)
				std::cout << "timeout" << std::endl;
			if (3 == id)
			{
				std::cout << "end thread id:" << std::this_thread::get_id() << std::endl;
				e[0].reset();
				break;
			}
		}
	});

	std::future<void> fs[5];
	for (auto& f : fs)
	{
		f = std::async(std::launch::async, [&e] {
			e[0].wait();
			std::cout << "entry thread id:" << std::this_thread::get_id() << std::endl;
			e[1].set();
			e[2].set();
		});
	}
	e[0].set();
	for (auto& f : fs)
		f.wait();
	f.wait();
}