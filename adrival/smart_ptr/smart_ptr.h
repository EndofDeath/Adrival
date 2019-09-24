#ifndef ADRIVAL_SMART_PTR_H
#define ADRIVAL_SMART_PTR_H
#pragma once

#include <type_traits>
#include "../utility/type_traits.h"

namespace adrival {

struct ref_count_base
{
	void incref()
	{
		use_count_++;
	}
	void decref()
	{
		use_count_--;
		if (0 == use_count_)
		{
			destroy();
			delete_();
		}
	}
	size_t use_count() const
	{
		return use_count_;
	}
private:
	virtual void destroy() = 0;
	virtual void delete_() = 0;
	size_t use_count_{ 1 };
};

template<class type>
struct ref_count :public ref_count_base
{
	ref_count(type* ptr)
		:ptr_(ptr)
	{}
	virtual void destroy()
	{
		delete ptr_;
	}
	virtual void delete_()
	{
		delete this;
	}
	type* ptr_;
};

template<class type,class deleter>
struct ref_count_resouce :public ref_count_base
{
	ref_count_resouce(type* ptr, deleter dt)
		:ptr_(ptr),dt_(std::move(dt))
	{}
	virtual void destroy()
	{
		dt_(ptr_);
	}
	virtual void delete_()
	{
		delete this;
	}
	type* ptr_;
	deleter dt_;
};

template<class T>
class smart_ptr
{
public:
	smart_ptr() noexcept
	{
	}

	~smart_ptr() noexcept
	{
		decref();
	}
	template<class type,class = std::enable_if_t<std::is_convertible_v<type*,T*>>>
	smart_ptr(type* ptr)
		: ptr_(ptr)
	{
		create(ptr);
	}
	template<class type, class deleter,class = std::enable_if_t<std::is_convertible_v<type*, T*>&&is_invocable<deleter,type*>::value>>
	smart_ptr(type* ptr,deleter dt)
		: ptr_(ptr)
	{
		create(ptr,std::move(dt));
	}
	smart_ptr(const smart_ptr& other)
	{
		copy(other);
	}
	template<class type, class = std::enable_if_t<std::is_convertible_v<type*, T*>>>
	smart_ptr(const smart_ptr<type>& other)
	{
		copy(other);
	}
	smart_ptr(smart_ptr&& other) noexcept
	{
		move(std::move(other));
	}
	template<class type, class = std::enable_if_t<std::is_convertible_v<type*, T*>>>
	smart_ptr(smart_ptr<type>&& other)
	{
		move(std::move(other));
	}
	smart_ptr& operator=(const smart_ptr& other)
	{
		smart_ptr(other).swap(*this);
		return *this;
	}
	template<class type, class = std::enable_if_t<std::is_convertible_v<type*, T*>>>
	smart_ptr& operator=(const smart_ptr<type>& other)
	{
		smart_ptr(other).swap(*this);
		return *this;
	}
	smart_ptr& operator=(smart_ptr&& other)
	{
		smart_ptr(std::move(other)).swap(*this);
		return *this;
	}
	template<class type, class = std::enable_if_t<std::is_convertible_v<type*, T*>>>
	smart_ptr& operator=(smart_ptr<type>&& other)
	{
		smart_ptr(std::move(other)).swap(*this);
		return *this;
	}
	T* get() const
	{
		return ptr_;
	}
	T* operator->() const
	{
		return ptr_;
	}
	bool has_value() const
	{
		return nullptr != ptr_;
	}
	void reset() noexcept
	{
		smart_ptr().swap(*this);
	}
	template<class type, class = std::enable_if_t<std::is_convertible_v<type*, T*>>>
	void reset(type* ptr)
	{
		smart_ptr(ptr).swap(*this);
	}
	template<class type, class deleter, class = std::enable_if_t<std::is_convertible_v<type*, T*>&&is_invocable<deleter, type*>::value>>
	void reset(type* ptr, deleter dt)
	{
		smart_ptr(ptr, std::move(dt)).swap(*this);
	}
	void swap(smart_ptr& other) noexcept
	{
		std::swap(ref_, other.ref_);
		std::swap(ptr_, other.ptr_);
	}
	size_t use_count() const
	{
		return ref_ ? ref_->use_count() : 0;
	}

private:
	
	template<class type>
	void create(type* ptr)
	{
		ref_ = new ref_count<type>(ptr);
	}
	template<class type,class deleter>
	void create(type* ptr, deleter dt)
	{
		ref_ = new ref_count_resouce<type, deleter>(ptr, std::move(dt));
	}
	void incref()
	{
		if(ref_)
			ref_->incref();
	}
	void decref()
	{
		if (ref_)
			ref_->decref();
	}
	template<class type>
	void copy(const smart_ptr<type>& other)
	{
		ref_ = other.ref_;
		ptr_ = other.ptr_;
		incref();
	}
	template<class type>
	void move(smart_ptr<type>&& other) noexcept
	{
		ref_ = other.ref_;
		other.ref_ = nullptr;
		ptr_ = other.ptr_;
		other.ptr_ = nullptr;
	}
	ref_count_base* ref_{};
	T* ptr_{};
	template<class type>
	friend class smart_ptr;
};

}
#endif // !ADRIVAL_SMART_PTR_H