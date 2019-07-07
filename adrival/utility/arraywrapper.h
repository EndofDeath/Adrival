#ifndef ADRIVAL_ARRAY_WRAPPER_H
#define ADRIVAL_ARRAY_WRAPPER_H

namespace adrival {

template<class T>
class arraywrapper
{
public:
	template<size_t N>
	arraywrapper(T(&arr)[N])
		:p(arr)
		,size_(N)
	{
	}
	T& operator[](size_t i)
	{
		if (i < size_) return p[i];
		throw "subscript out of range";
	}
	const T& operator[](size_t i) const
	{
		if (i < size_) return p[i];
		throw "subscript out of range";
	}
	T* begin() const
	{
		return p;
	}
	T* end() const
	{
		return p + size_;
	}
	size_t size() const
	{
		return size_;
	}
private:
	T* p;
	size_t size_;
};

template<class T,size_t N>
auto make_arraywrapper(T(&arr)[N])
{
	return arraywrapper<T>(arr);
}

}
#endif // !ADRIVAL_ARRAY_WRAPPER_H