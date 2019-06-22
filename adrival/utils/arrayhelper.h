#ifndef ADRIVAL_ARRAYHELPER_H
#define ADRIVAL_ARRAYHELPER_H

namespace adrival {

template<class T,size_t N>
constexpr size_t size(T(&arr)[N])
{
	return N;
}

template<class T, size_t N>
T* end(T(&arr)[N])
{
	return 1[&arr];
}

}
#endif // !ADRIVAL_ARRAYHELPER_H