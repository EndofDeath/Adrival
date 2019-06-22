#include <iostream>
#include <thread>
#include "concurrent/Semaphore.h"


void test_semaphore()
{
	adrival::semaphore s(0, 5);
	std::thread ths[15];
	for (auto& t : ths)
	{
		t = std::thread([&s] {
			s.wait();
			std::cout << "thread id:" << std::this_thread::get_id() << std::endl;
			s.release(2);
		});
	}
	s.release(1);
	for (auto& t : ths)
		t.join();
}