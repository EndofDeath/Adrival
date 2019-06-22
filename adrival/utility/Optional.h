#ifndef ADRIVAL_OPTIONAL_H
#define ADRIVAL_OPTIONAL_H
#pragma once

#include <type_traits>
#include <exception>

namespace adrival {

template<typename T>
class optional
{
	using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
public:
	optional() : has_value_(false) {}
	optional(const T& v)
	{
		Create(v);
	}

	optional(T&& v) : has_value_(false)
	{
		Create(std::move(v));
	}

	~optional()
	{
		Destroy();
	}

	optional(const optional& other) : has_value_(false)
	{
		if (other.has_value_)
			Assign(other);
	}

	optional(optional&& other) : has_value_(false)
	{
		if (other.has_value_)
		{
			Assign(std::move(other));
			other.Destroy();
		}
	}

	template<class T2 = T, class = std::enable_if < std::is_same < T, std::decay<T2>::type > ::value > ::type >
	optional& operator=(T2&& v)
	{
		if (has_value_)
			*(T*)(&data_) = std::forward<T>(v);
		else
			Create(std::forward<T>(v));
		return *this;
	}

	optional& operator=(optional &&other) noexcept
	{
		Assign(std::move(other));
		return *this;
	}

	optional& operator=(const optional &other)
	{
		Assign(other);
		return *this;
	}

	template<class... Args>
	T& emplace(Args&&... args)
	{
		Destroy();
		Create(std::forward<Args>(args)...);
		return value();
	}

	bool has_value() const { return has_value_; }

	explicit operator bool() const {
		return has_value();
	}

	T& operator*()
	{
		return value();
	}

	T const& operator*() const
	{
		return value();
	}

	T* operator->()
	{
		return ((T*)(&data_));
	}

	const T* operator->() const
	{
		return ((T*)(&data_));
	}

	bool operator == (const optional<T>& rhs) const
	{
		return (!bool(*this)) != (!rhs) ? false : (!bool(*this) ? true : (*(*this)) == (*rhs));
	}

	bool operator < (const optional<T>& rhs) const
	{
		return !rhs ? false : (!bool(*this) ? true : (*(*this) < (*rhs)));
	}

	bool operator != (const optional<T>& rhs)
	{
		return !(*this == (rhs));
	}

	void reset()
	{
		Destroy();
	}

	T& value()
	{
		if (has_value_)
		{
			return *(T*)(&data_);
		}
		throw std::exception("accessing an optional object that does not contain a value.");
	}

	const T& value() const
	{
		if (has_value_)
		{
			return *(T*)(&data_);
		}
		throw std::exception("accessing an optional object that does not contain a value.");
	}

	void swap(optional& other)
	{
		if (!has_value_ && !other.has_value_)
			return;
		if (!has_value_ && other.has_value_)
		{
			Assign(std::move(other));
		}
		else if (has_value_ && !other.has_value_)
		{
			other.Assign(std::move(*this));
		}
		else
			std::swap(**this, *other);
	}

private:
	template<class... Args>
	void Create(Args&&... args)
	{
		new (&data_) T(std::forward<Args>(args)...);
		has_value_ = true;
	}

	void Destroy()
	{
		if (has_value_)
		{
			has_value_ = false;
			if(std::is_class<T>::value)
				((T*)(&data_))->~T();
		}
	}

	void Assign(const optional& other)
	{
		if (other.has_value_)
		{
			Copy(other.data_);
			has_value_ = true;
		}
		else
		{
			Destroy();
		}
	}

	void Assign(optional&& other)
	{
		if (other.has_value_)
		{
			Move(std::move(other.data_));
			has_value_ = true;
			other.Destroy();
		}
		else
		{
			Destroy();
		}
	}

	void Move(data_t&& val)
	{
		Destroy();
		new (&data_) T(std::move(*((T*)(&val))));
	}

	void Copy(const data_t& val)
	{
		Destroy();
		new (&data_) T(*((T*)(&val)));
	}

private:
	data_t data_{};
	bool has_value_;
};

}
#endif // !ADRIVAL_OPTIONAL_H