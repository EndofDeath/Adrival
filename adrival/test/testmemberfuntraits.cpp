
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

	HAS_MEMFN(init)
	HAS_MEMFN(print)

	HAS_EXPLICIT_MEMFN(init)
	HAS_EXPLICIT_MEMFN(print)
}


void test_memberfuntrait()
{
	has_memfn_init<one, void, int>::value;
	has_memfn_init<one>::value;
	has_memfn_init<empty>::value;

	has_memfn_init<base>::value;
	has_memfn_init<deriveda>::value;
	has_memfn_init<derivedb>::value;
	has_memfn_init<derivedc>::value;
	has_memfn_init<derivedd>::value;

	has_memfn_print<one, void, int>::value;
	has_memfn_print<one>::value;
	has_memfn_print<empty>::value;

	has_memfn_print<base>::value;
	has_memfn_print<deriveda>::value;
	has_memfn_print<derivedb>::value;
	has_memfn_print<derivedc>::value;
	has_memfn_print<derivedd>::value;

	has_explicit_memfn_init<base>::value;
	has_explicit_memfn_init<deriveda>::value;
	has_explicit_memfn_init<derivedb>::value;
	has_explicit_memfn_init<derivedc>::value;
	has_explicit_memfn_init<derivedd>::value;

	has_explicit_memfn_print<base>::value;
	has_explicit_memfn_print<deriveda>::value;
	has_explicit_memfn_print<derivedb>::value;
	has_explicit_memfn_print<derivedc>::value;
	has_explicit_memfn_print<derivedd>::value;
}