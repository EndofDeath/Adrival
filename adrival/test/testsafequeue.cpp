#include <iostream>
#include "../safequeue.hpp"

namespace {

	adrival::safequeue<int> data;
	thread_local int i = 10;
	int store[20]{};

}

void test_safequeue()
{
	std::thread writes[2];
	std::thread reads[4];
	for (auto& t : writes)
		t = std::thread([] {
		i += 10;
		while (i--)
		{
			data.push(i);
		}
	});

	for (auto& t : reads)
		t = std::thread([] {
		while (i)
		{
			int v{};
			if (data.try_pop(v))
			{
				store[v]++;
				i--;
			}
			else
				std::this_thread::yield();
		}
	});

	for (auto& t : writes)
		t.join();
	for (auto& t : reads)
		t.join();

	for (auto i : store)
		std::cout << i << std::endl;
}