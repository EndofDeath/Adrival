
#include "utility/simpletuple.h"

void test_simpletuple()
{
	using namespace adrival;
	simpletuple<int, char> sp(5, 'a');
	auto v1 = get<0>(sp);
	auto v2 = get<1>(sp);
	auto v3 = get<0>(make_tuple(5, 78));
}