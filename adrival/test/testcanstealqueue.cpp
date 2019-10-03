#include <iostream>
#include <atomic>
#include "../canstealqueue.hpp"
#include "../thread_guards.hpp"

static std::atomic_bool done = true;

void test_canstealqueue()
{
	using namespace adrival;

	cansteal_queue<int> tq;
	std::thread ts([&tq] {
		for (int i = 0; i < 1000; i++)
		{
			tq.push(i);
			printf_s("push %d ", i);
		}
	});
	cansteal_queue<int> tq1;

	thread_guards threads;
	threads.create(4);
	for (auto&t : threads)
		t = std::thread([&tq, &tq1] {
		int i{};
		while (done)
		{
			if (tq1.try_pop(i) || tq.try_steal(i))
				printf_s("pop %d ", i);
			else
				std::this_thread::yield();
		}
	});

	int i{};
	while (tq.try_pop(i))
	{
		printf_s("m pop %d ", i);
		tq1.push(i + 1000);
		printf_s("m push %d ", i + 1000);
	}
	ts.join();
	done = false;
	//for (auto&t : threads)
	//	t.join();

	tq.try_pop(i);
}