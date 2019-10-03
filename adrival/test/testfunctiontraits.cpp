#include <iostream>
#include "../function_traits.hpp"

namespace {

	class
	{
	public:
		void operator()(const int &i)
		{
			std::cout << i << std::endl;
		}
	}unnamed;

	class objecta
	{
	public:
		virtual void print()
		{
			std::cout << "objecta" << std::endl;
		}

		void operator()(const int& i)
		{
			std::cout << "objecta" << i << std::endl;
		}
	};

	template<class T>
	int callback(int(*proc)(T), T data)
	{
		return proc(data);
	}

	void show(char c, short s, int i, double f)
	{
		std::cout << c << "  " << s << "  " << i << "  " << f << std::endl;
	}

}

using adrival::function_traits;

#define farg(fun,n) \
	using fun##arg##n = function_traits<decltype(fun)>::args<n>::type; \
	fun##arg##n

using showarg1 = function_traits<decltype(show)>::args<1>::type;

void test_functiontrait()
{
	function_traits<decltype(show)>::args<0>::type a{ 'a' };

	showarg1 b{ a };

	farg(show, 0) c { 'c' };
	farg(show, 3) d { 'c' };

	int ib = 0;

	function_traits<decltype(unnamed)>::args<0>::type r = ib;

	function_traits<decltype(callback<int>)>::return_type ret = r;

	function_traits<decltype(callback<int>)>::function_reference fr = callback<int>;

	function_traits<objecta>::args<0>::type ri = 1;

	function_traits<objecta>::class_type ct;

	function_traits<objecta>::memfun_pointer mfp = nullptr;

	auto l = [](int i, char c) {return c + i; };
	farg(l, 1) ca { 'c' };
}