#include <iostream>
#include "concurrent/futuretask.h"

using namespace std::literals;

using adrival::futuretask;

void test_futuretask()
{
	std::promise<int> p;
	futuretask<int> f1 = p.get_future();

	futuretask<char> f2([] {
		std::cout << "sleep" << std::endl;
		std::this_thread::sleep_for(10ms);
		std::cout << "wake" << std::endl;
		return 'a';
	});

	auto ftp = when_all(f1, f2).then([](int i, char c) {
		std::cout << "get 1 int" << std::endl;
		std::cout << i << std::endl;
		std::cout << "get 2 char" << std::endl;
		std::cout << c << std::endl;
		return "test";
	}).then([](const char*p) {
		std::cout << "get string" << std::endl;
		std::cout << p << std::endl;
		return 10;
	}).then(std::bind([](int i, char c) {
		std::cout << "get int" << std::endl;
		std::cout << i << std::endl;
		std::cout << "get char" << std::endl;
		std::cout << c << std::endl;
	}, std::placeholders::_1, 'b'));

	p.set_value(5);
	std::cout << "set value" << std::endl;

	ftp.wait();

	futuretask<void> ft([] {
		std::cout << "begin" << std::endl;
	});
	ft.then([] {
		std::cout << "step 1" << std::endl;
		return 0;
	}).then([] (int i){
		std::cout << "step 2" << std::endl;
		std::cout << i << std::endl;
		return "get";
	}).then([](const char*p) {
		std::cout << "step 3" << std::endl;
		std::cout << p << std::endl;
	}).then([] {
		std::cout << "step 4" << std::endl;
	});

	ft = std::async([] {
		std::cout << "new step 1" << std::endl;
	});
	ft.thenschedule([] {
		std::cout << "new step 2" << std::endl;
	}, [] {
		std::cout << "new step 3" << std::endl;
	}, [] {
		std::cout << "new step 4" << std::endl;
	}, [] {
		std::cout << "new step 5" << std::endl;
	});
}