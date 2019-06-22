#ifndef ADRIVAL_ANY_H
#define ADRIVAL_ANY_H
#pragma once

#include <typeindex>
#include <memory>

namespace adrival{

class any
{
public:
	any() :value_(nullptr){}
	~any(){}

	template<class T>
	bool IsType()
	{
		return value_?(value_->type() == typeid(typename std::decay<T>::type)):false;
	}

private:
	class implbase;
	using basePtr = std::unique_ptr < implbase > ;
	class implbase
	{
	public:
		virtual ~implbase(){};
		virtual implbase* copy() const = 0;
		virtual const std::type_info& type() const = 0;
	};

	template <class T>
	class implT :public implbase
	{
	public:
		implT(const T& v) :value(v){}
		const std::type_info& type() const
		{
			return typeid(T);
		}
		implbase* copy() const
		{
			return new implT<T>(value);
		}
		T value;
	};

	basePtr copy() const
	{
		if (value_)
		{
			return basePtr(value_->copy());
		}
		return nullptr;
	}
	basePtr value_;
public:
	template<class T,class = std ::enable_if<!std::is_same<typename std::decay<T>::type,any>::value>::type>
	any(T&& v) :value_(new implT<typename std::decay<T>::type>(v)){}

	any(const any& rhs) :value_(rhs.copy()){}
	any(any&& rhs) noexcept :value_(std::move(rhs.value_)) {}

	any& operator=(const any& rhs)
	{
		if (value_ == rhs.value_)
			return *this;
		value_ = rhs.copy();
		return *this;
	}

	any& operator=(any&& rhs) noexcept
	{
		value_ = std::move(rhs.value_);
		return *this;
	}

	template<class T, class = std::enable_if<!std::is_same<typename std::decay<T>::type, any>::value>::type>
	any& operator=(T&& v)
	{
		*this = any(std::forward<T>(v));
		return *this;
	}

	template<class T>
	operator T()
	{
		if (IsType<T>())
		{
			auto t = dynamic_cast<implT<T>*>(value_.get());
			return t->value;
		}
		throw std::bad_cast::__construct_from_string_literal("Bad any cast");
	}

	bool has_value() const
	{
		return (bool)value_;
	}

	void reset()
	{
		value_.reset();
	}

	void swap(any& other)
	{
		value_.swap(other.value_);
	}

	const std::type_info& type() const
	{
		return value_?value_->type():typeid(void);
	}
};

}
#endif // !ADRIVAL_ANY_H