#ifndef ADRIVAL_INDEX_SQUENCE_H
#define ADRIVAL_INDEX_SQUENCE_H

#include <stddef.h>
namespace adrival{

template <size_t... indexs>
struct indexsequence {};

template <size_t... indexs>
struct index_helper;

template <size_t size, size_t... indexs>
struct index_helper<size, indexs...>
{
	using type = typename index_helper<size - 1, size - 1, indexs...>::type;
};

template <size_t... indexs>
struct index_helper<0, indexs...>
{
	using type = indexsequence< indexs...>;
};

template<size_t size>
typename index_helper<size>::type make_indexsequence()
{
	return index_helper<size>::type();
}

template <class...Args>
using indexsequence_for = decltype(make_indexsequence<sizeof...(Args)>());

}
#endif // !ADRIVAL_INDEX_SQUENCE_H