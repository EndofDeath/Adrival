#include <iostream>
#include "../taskpool.hpp"

namespace {

	adrival::taskpool tps;
	static constexpr auto MAXCOUNT = 100u;
	int id[MAXCOUNT]{};
	std::thread::id tid[MAXCOUNT]{};

}

void test_taskpool()
{
	tps.submit([] {
		std::cout << std::this_thread::get_id() << std::endl;
	});

	auto t = std::thread([] {
		int count = MAXCOUNT;
		while (count--)
		{
			auto f = tps.submitpackage([count] {
				id[count]++;
				tid[count] = std::this_thread::get_id();
				//tps.submit([] {
				//	std::cout << std::this_thread::get_id() << std::endl;
				//});
			});
			auto s = f.share();
			tps.submitpackage([s] {
				s.wait();
				std::cout << std::this_thread::get_id() << std::endl;
			}).wait();
		}
		std::cout << std::this_thread::get_id() << std::endl;
	});
	tps.try_run_task();

	t.join();

	for (auto&& t: id) {
		std::cout << t << std::endl;
	}
	for (auto&& t : tid) {
		std::cout << t << std::endl;
	}
}