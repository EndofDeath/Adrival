
#include "utility/simpletuple.h"

struct first
{
	static constexpr size_t value = 0;
};

struct second
{
	static constexpr size_t value = 1;
};

void test_simpletuple()
{
	using namespace adrival;
	simpletuple<int, char> sp(5, 'a');
	auto v1 = get<0>(sp);
	auto v2 = get<1>(sp);
	auto v3 = get<0>(make_tuple(5, 78));

	auto v4 = sp[std::integral_constant<size_t, 0>{}];
	auto v5 = sp[first{}];
	auto v6 = sp[second{}];
}