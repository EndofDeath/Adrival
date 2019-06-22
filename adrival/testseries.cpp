
#include <iostream>
#include <thread>
#include <mutex>
#include "utils/series.h"

using namespace std::literals;

static std::mutex mut;
static thread_local auto i = 10;

void test_series()
{
	std::thread ts[5];
	int v(1);
	for (auto&&t : ts) {
		t = std::thread([v = v++]{
			auto c = adrival::AP(v,5);
			while (i--)
			{
				{
					std::lock_guard<std::mutex> l(mut);
					std::cout << std::this_thread::get_id() << "--" << c() << std::endl;
				}
				std::this_thread::sleep_for(10ms);
			}
			});
	}
	for (auto&&t : ts) {
		t.join();
	}
	std::cout << v << std::endl;
}