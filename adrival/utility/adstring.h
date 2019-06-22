#pragma once
#include<initializer_list>
#include<stddef.h>

namespace adrival{

class adstring
{
public:
	using iterator = char*;
	adstring();
	~adstring();
	adstring(const adstring& rhs);
	adstring(const char* res, size_t l);
	adstring(const std::initializer_list<char> &anych);
	template<size_t N>
	adstring(char(&arr)[N])
	{
		init(N);
		iterator t = begin();
		for (auto c : arr)
		{
			*t++ = c;
		}
		size_ -= (0 == arr[N - 1]);
	}
	adstring& operator=(const adstring& rhs) = delete;
	adstring& operator+=(const adstring& rhs);
	adstring& operator+=(const std::initializer_list<char> &anych);
	template<size_t N>
	adstring& operator+=(char(&arr)[N])
	{
		size_t st = size_;
		addsize(N - (0 == arr[N - 1]));
		iterator t = begin()+st;
		for (auto c : arr)
		{
			*t++ = c;
		}
		return *this;
	}
	char* begin() const;
	char* end() const;
	size_t size() const;
	char operator[](size_t index) const;
	char& operator[](size_t index);
private:
	void init(size_t csize = 0);
	void addsize(size_t csize = 1);
	char *buffer_{};
	size_t size_{};
};

}