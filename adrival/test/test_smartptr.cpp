#include <iostream>
#include <memory>
#include "../smart_ptr.hpp"

class base
{
public:
	base()
	{
		std::cout << "base instructor" << std::endl;
	}

	~base()
	{
		std::cout << "base destructor" << std::endl;
	}
};

class derived:public base
{
public:
	derived()
	{
		std::cout << "derived instructor" << std::endl;
	}

	~derived()
	{
		std::cout << "derived destructor" << std::endl;
	}
};

class derived2 :public derived
{
public:
	derived2()
	{
		std::cout << "derived2 instructor" << std::endl;
	}

	~derived2()
	{
		std::cout << "derived2 destructor" << std::endl;
	}
};

using adrival::smart_ptr;

template<class T>
std::shared_ptr<T> make_shared(smart_ptr<T>& p)
{
	return std::shared_ptr<T>(p.get(), [p = p](void const*)mutable {p.reset(); });
}

template<class T>
smart_ptr<T> make_smart(std::shared_ptr<T>& p)
{
	return smart_ptr<T>(p.get(), [p = p](void const*)mutable {p.reset(); });
}

void test_smartptr()
{
	{
		smart_ptr<base> p1 = new derived;
	}
	std::cout << "test1 end" << std::endl;
	{
		smart_ptr<base> p1 = new derived2;
		smart_ptr<void> p2;
		p2 = p1;
		p1.reset();
	}
	std::cout << "test2 end" << std::endl;
	{
		smart_ptr<void> p2 = new derived2;
		try
		{
			auto p3 = make_shared(p2);
			p2.use_count();
		}
		catch (const std::exception&)
		{
		}
		p2.use_count();
	}
	std::cout << "test3 end" << std::endl;
	{
		std::shared_ptr<void> p1;
		try
		{
			smart_ptr<void> p2 = new derived2;
			p1 = make_shared(p2);
			p2.use_count();
		}
		catch (const std::exception&)
		{
		}
	}
	std::cout << "test4 end" << std::endl;
	{
		std::shared_ptr<void> p1(new derived2);
		try
		{
			smart_ptr<void> p2;
			p2 = make_smart(p1);
			p1.use_count();
		}
		catch (const std::exception&)
		{
		}
		p1.use_count();
	}
	std::cout << "test5 end" << std::endl;
	{
		smart_ptr<void> p1;
		try
		{
			std::shared_ptr<void> p2(new derived2);
			p1 = make_smart(p2);
			p2.use_count();
		}
		catch (const std::exception&)
		{
		}
	}
	std::cout << "test6 end" << std::endl;
}