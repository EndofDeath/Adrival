#ifndef ADRIVAL_OBJECT_POOL_H
#define ADRIVAL_OBJECT_POOL_H
#pragma once
#include <memory>
#include <list>
#include <functional>

namespace adrival{

template <class T>
class recycle_ptr
{
	using Deleter = std::function<void(T*)>;
public:
	recycle_ptr()
	{
	}

	recycle_ptr(T* p, Deleter d)
		:ptr(p)
		, dx(std::move(d))
	{
	}

	recycle_ptr(recycle_ptr&& other) noexcept
		:ptr(std::move(other.ptr))
		, dx(std::move(other.dx))
	{
	}

	recycle_ptr& operator=(recycle_ptr&& other) noexcept
	{
		ptr = std::move(other.ptr);
		dx = std::move(other.dx);
		return *this;
	}

	recycle_ptr(const recycle_ptr&) = delete;
	recycle_ptr& operator=(const recycle_ptr&) = delete;

	~recycle_ptr()
	{
		reset();
	}
	T* get() const
	{
		return ptr.get();
	}
	T* operator->() const
	{
		return ptr.get();
	}
	void reset()
	{
		if (ptr&&dx)
			std::unique_ptr<T, Deleter> t(ptr.release(), dx);
	}
private:
	std::unique_ptr<T> ptr;
	Deleter dx;
};

template <class T>
class objectpool
{
public:
	objectpool(){}
	void add(T* p)
	{
		recycle_.push_back(std::unique_ptr<T>(p));
	}

	recycle_ptr<T> create()
	{
		std::unique_ptr<T> p;
		if (recycle_.empty())
		{
			p.reset(new T());
		}
		else
		{
			p.swap(recycle_.front());
			recycle_.pop_front();
		}

		recycle_ptr<T> ptr(p.release(), [this](T* t)
		{
			add(t);
		});
		return std::move(ptr);
	}

	std::shared_ptr<T> create_shared()
	{
		std::unique_ptr<T> p;
		if (recycle_.empty())
		{
			p.reset(new T());
		}
		else
		{
			p.swap(recycle_.front());
			recycle_.pop_front();
		}

		return std::shared_ptr<T>(p.release(), [this](T* t)
		{
			add(t);
		});
	}

	bool empty() const
	{
		return recycle_.empty();
	}

	size_t size() const
	{
		return recycle_.size();
	}
	objectpool(const objectpool&) = delete;
	objectpool& operator=(const objectpool&) = delete;
private:
	std::list<std::unique_ptr<T>> recycle_;
};

}
#endif//ADRIVAL_OBJECT_POOL_H