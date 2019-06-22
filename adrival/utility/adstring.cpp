
#include "adstring.h"
namespace adrival {

adstring::adstring()
	:buffer_(nullptr), size_(0)
{
	init();
}

adstring::~adstring()
{
	delete[] buffer_;
}

adstring::adstring(const adstring& rhs)
{
	init(rhs.size_);
	iterator t = begin();
	for (auto c : rhs)
	{
		*t++ = c;
	}
}

adstring::adstring(const char* res, size_t l)
{
	init(l);
	iterator t = const_cast<char*>(res);
	for (auto &i : *this)
	{
		i = *t++;
	}
}

adstring::adstring(const std::initializer_list<char> &anych)
	:buffer_(nullptr), size_(0)
{
	init(anych.size());
	iterator t = begin();
	for (auto c : anych)
	{
		*t++ = c;
	}
}

void adstring::init(size_t csize)
{
	buffer_ = new char[csize + 1]{0};
	size_ = csize;
	buffer_[size_] = 0;
}

char* adstring::begin() const
{
	return buffer_;
}

char* adstring::end() const
{
	return buffer_+size_;
}

size_t adstring::size() const
{
	return size_;
}

adstring& adstring::operator+=(const std::initializer_list<char> &anych)
{
	size_t st = size_;
	addsize(anych.size());
	iterator t = begin() + st;
	for (auto i : anych)
	{
		*t++ = i;
	}
	return *this;
}

adstring& adstring::operator+=(const adstring &rhs)
{
	size_t st = size_;
	addsize(rhs.size());
	iterator t = begin() + st;
	for (auto i : rhs)
	{
		*t++ = i;
	}
	return *this;
}

void adstring::addsize(size_t csize)
{
	if (0 == csize)
		return;
	char* ptemp = new char[size_ + csize + 1]{0};
	char* pbegin = ptemp;
	for (auto i : *this)
	{
		*pbegin++ = i;
	}
	delete[] buffer_;
	size_ += csize;
	buffer_ = ptemp;
	buffer_[size_] = 0;
}

char adstring::operator[](size_t index) const
try
{
	if (size_ < index)
		throw 1;
	return buffer_[index];
}
catch (...)
{

}

char& adstring::operator[](size_t index)
try
{
	if (size_ <= index)
		throw 1;
	return buffer_[index];
}
catch (...)
{

}

}