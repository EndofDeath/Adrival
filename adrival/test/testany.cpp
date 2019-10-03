
#include <iostream>
#include "../any.hpp"

namespace {

	class deriveda
	{
	public:
		void print()
		{
			std::cout << "deriveda" << std::endl;
		}
	};

	using AnyType = adrival::any;

	void changeint(AnyType a)
	{
		try
		{
			int *p = a;
			*p = 10;
		}
		catch (const std::exception& e)
		{
			std::wcout << e.what() << std::endl;
		}
	}

	int funa(bool)
	{
		return 1;
	}

}

void test_any()
{
	AnyType a = 1;
	AnyType b = 'a';
	AnyType c = 3.14;

	int array[3][4]{};
	AnyType arr = array;
	int(*parr)[4] = arr;
	AnyType fun1 = funa;
	AnyType fun2 = fun1;

	int(*pfun)(bool) = fun1;
	pfun(true);

	AnyType w = deriveda();
	deriveda some[5];
	AnyType psome = some;
	deriveda* const pas = psome;

	int ib = a;
	changeint(&ib);

	AnyType var;
	var = "abc";
	const char* pabc = var;
	a.swap(b);
	a = 'a';

	auto fun3 = [](AnyType v) {int* p = v; *p = 10; };
	fun3(&ib);

	int&& r1 = 1;
	AnyType ra = r1;
	int &r2 = ib;
	AnyType rb = r2;
	const char cd = a;
	AnyType cc = cd;
	char&& rd = cc;
	try {
		ib = b;
	}
	catch (std::exception error)
	{
		std::cout << error.what() << std::endl;
	}

	char* p = nullptr;
	try {
		p = var;
	}
	catch (std::exception error)
	{
		std::wcout << error.what() << std::endl;
	}

	auto l = [](int i) {return i + 1; };
	AnyType lf = l;
	decltype(l) f1 = lf;

	AnyType more[100];
	for (auto& i : more)
	{
		i = "abcdefg";
	}
	for (auto& i : more)
	{
		i.reset();
	}
	for (auto& i : more)
	{
		i = ra;
	}
}