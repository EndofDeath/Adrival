#include <iostream>
#include <thread>
#include "../lockfree.hpp"

static thread_local auto i = 100;

void test_lockfreequeue()
{
	using adrival::lockfreequeue;

	lockfreequeue<int> lfq;

	std::thread push([&lfq] {
		while (i > 0)
		{
			lfq.push(i--);
		}
	});

	
	std::thread pop1([&lfq] {
		int ret;
		i = 50;
		while (i-- > 0)
		{
			lfq.try_pop(ret);
			std::cout << ret << std::endl;
		}
	});

	std::thread pop2([&lfq] {
		int ret;
		i = 50;
		while (i-- > 0)
		{
			lfq.try_pop(ret);
			std::cout << ret << std::endl;
		}
	});

	push.join();
	pop1.join();
	pop2.join();

	int ret;
	lfq.try_pop(ret);
}