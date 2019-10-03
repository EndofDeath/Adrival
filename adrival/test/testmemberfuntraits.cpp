
#include <iostream>
#include "../memberfun_traits.hpp"

namespace {

	class empty
	{
	};

	class one
	{
	public:
		void init()
		{
			std::cout << typeid(*this).name() << std::endl;
		}

		void print() const
		{}

		void print(int i) const
		{
			std::cout << i << std::endl;
		}
	};

	class base
	{
	public:
		void init()
		{

		}

		virtual void print()
		{
			std::cout << "base" << std::endl;
		}
	};

	class deriveda :virtual public base
	{
	public:
		void print()
		{
			std::cout << "deriveda" << std::endl;
		}
	};

	class derivedb :public base
	{
	public:
		void print()
		{
			std::cout << "derivedb" << std::endl;
		}
	};

	class derivedc :virtual public base
	{
	public:
		void print()
		{
			std::cout << "derivedc" << std::endl;
		}
	};

	class derivedd :public deriveda, public derivedc
	{
	public:
		void print()
		{
			std::cout << "derivedd" << std::endl;
		}
	};

	has_men_fn(init)
	has_men_fn(print)

}


void test_memberfuntrait()
{
	has_mem_fn_init<one, void, int>::value;
	has_mem_fn_init<one>::value;
	has_mem_fn_init<empty>::value;

	has_mem_fn_init<base>::value;
	has_mem_fn_init<deriveda>::value;
	has_mem_fn_init<derivedb>::value;
	has_mem_fn_init<derivedc>::value;
	has_mem_fn_init<derivedd>::value;

	has_mem_fn_print<one, void, int>::value;
	has_mem_fn_print<one>::value;
	has_mem_fn_print<empty>::value;

	has_mem_fn_print<base>::value;
	has_mem_fn_print<deriveda>::value;
	has_mem_fn_print<derivedb>::value;
	has_mem_fn_print<derivedc>::value;
	has_mem_fn_print<derivedd>::value;
}