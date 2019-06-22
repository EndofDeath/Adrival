#ifndef ADRIVAL_PODPLUS_H
#define ADRIVAL_PODPLUS_H

#include <type_traits>

namespace adrival{

template<class T, T N, class = std::enable_if<std::is_pod<T>::value>::type>
class podplus
{
public:
	podplus(T i = 0) :value(i) {}
	podplus& operator++() 
	{
		value += N;
		return *this;
	}
	const podplus operator++(int) 
	{
		podplus tmp = *this;
		value += N;
		return tmp;
	}
	podplus& operator--() 
	{
		value -= N;
		return *this;
	}
	const podplus operator--(int)
	{
		podplus tmp = *this;
		value -= N;
		return tmp;
	}
	operator T() const
	{
		return value;
	}
	podplus& operator=(T i)
	{
		value = i;
		return *this;
	}
private:
	T value;
};

}
#endif // !ADRIVAL_PODPLUS_H