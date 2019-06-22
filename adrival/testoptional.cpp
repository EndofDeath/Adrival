#include <string>
#include "utility/Optional.h"

template<class T>
using Optional = adrival::optional<T>;

namespace {

	struct Person
	{
		std::string name;
		int age{};
		std::string city;
	};

	Optional<int> getint(bool b)
	{
		if (b)
			return 1;
		return{};
	}

}

void test_optional()
{
	Optional<int> a;
	//int ia = *a;
	a = 10;
	int b = a.value();
	a = getint(false);
	Optional<int> c{ 8 };
	c.swap(a);
	//c = ia;

	Optional<Person> p;
	p = Person();
	p->age = 10;
	p->city = "city";
	p->name = "a";
	Optional<Person> p2 = p;
	Optional<char> oa;
	oa = 'b';
}