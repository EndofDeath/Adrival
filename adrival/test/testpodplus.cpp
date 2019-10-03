#include <iostream>
#include <numeric>
#include "../podplus.hpp"

using adrival::podplus;

void test_podplus()
{
	podplus<int, -2> va(20);
	int array[5]{};
	std::iota(array, 1[&array], va);

	podplus<int, 2> vb(10);
	std::iota(array, 1[&array], vb);

	while (va >= -100)
	{
		std::cout << va++ << std::endl;
	}

	while (vb <= 100)
	{
		std::cout << vb++ << std::endl;
	}
}