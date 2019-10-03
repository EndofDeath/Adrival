#include <iostream>
#include "../polymorphic.hpp"

namespace {

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

	operator int()
	{
		return 1;
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

	void operator()(const int& i)
	{
		std::cout << "base" << i << std::endl;
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

IMPLEMENT_CONCEPT(print)
IMPLEMENT_CONCEPT(init)

void shared_print(concept_print<> v)
{
	v.print();
}

}

void test_concept()
{
	base b;
	concept_print<> cp = b;
	cp.print();

	deriveda da;
	cp = da;
	cp.print();

	derivedb db;
	cp = db;
	cp.print();

	derivedb dc;
	cp = dc;
	cp.print();

	derivedb dd;
	cp = dd;
	cp.print();
	
	one the;
	concept_print<void, int> cpi = the;
	cpi.print(10);
	//cpi = b; //error

	shared_print(b);
	shared_print(da);
	shared_print(db);
	shared_print(dc);
	shared_print(dd);

	base& ra = da;
	shared_print(ra);
	ra = db;
	shared_print(ra);
	ra = dc;
	shared_print(ra);
	ra = dd;
	shared_print(ra);
}