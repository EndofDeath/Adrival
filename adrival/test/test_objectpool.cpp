#include <iostream>
#include "../objectpool.hpp"

namespace
{
	class A
	{
	public:
		A()
		{
			std::cout << "class A ctor" << std::endl;
		}

		~A()
		{
			std::cout << "class A dtor" << std::endl;
		}
	};

}

template<class T>
std::ostream& operator<<(std::ostream& out, const adrival::recycle_ptr<T>& ptr)
{
	out << ptr.get();
	return out;
}


void test_obectpool()
{
	using namespace adrival;
	{
		std::list<recycle_ptr<A>> list;
		objectpool<A> pool;
		{
			auto p = pool.create();
			std::cout << p.get() << std::endl;
			std::cout << pool.create() << std::endl;
		}

		{
			std::cout << pool.create() << std::endl;
			std::cout << pool.create() << std::endl;
		}
		{
			std::cout << pool.create_shared() << std::endl;
			std::cout << pool.create_shared() << std::endl;
		}

		{
			list.push_back(pool.create());
			list.push_back(pool.create());
		}
		list.clear();
		std::cout << pool.size() << std::endl;
	}
}