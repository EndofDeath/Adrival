#ifndef ADRIVAL_SIMPLE_TUPLE_H
#define ADRIVAL_SIMPLE_TUPLE_H

#include <stddef.h>
#include <type_traits>

namespace adrival {

template<class...Type>
class simpletuple;

template<>
class simpletuple<>
{};

template<class This,class...Types>
class simpletuple<This, Types...>:public simpletuple<Types...>
{
	using base = simpletuple<Types...>;
public:
	simpletuple()
		:base()
		,value()
	{}
	simpletuple(const simpletuple& other) = default;

	simpletuple(This&& t, Types&& ...rest)
		:base(std::forward<Types>(rest)...)
		,value(std::forward<This>(t))
	{}
	simpletuple(const This& t, const Types& ...rest)
		:base(rest...)
		, value(t)
	{}
	This value;
};

template<size_t index,class T>
class simpletuple_element;

template<size_t index>
class simpletuple_element<index, simpletuple<>>
{
public:
	using valuetype = void;
	using thistype = simpletuple<>;
};

template<class This, class...Types>
class simpletuple_element <0, simpletuple<This, Types...>>
{
public:
	using valuetype = This;
	using thistype = simpletuple<This, Types...>;
};

template<size_t index, class This, class...Types>
class simpletuple_element < index, simpletuple<This, Types...>>:public simpletuple_element < index-1, simpletuple<Types...>>
{};


	template<size_t index, class...Types>
	typename simpletuple_element<index, simpletuple<Types...>>::valuetype& get(simpletuple<Types...>& t)
	{
		using type = typename simpletuple_element<index, simpletuple<Types...>>::thistype;
		return ((type&)t).value;
	}

	template<size_t index, class...Types>
	typename const simpletuple_element<index, simpletuple<Types...>>::valuetype& get(const simpletuple<Types...>& t)
	{
		using type = typename simpletuple_element<index, simpletuple<Types...>>::thistype;
		return ((type&)t).value;
	}

	template<size_t index, class...Types>
	typename simpletuple_element<index, simpletuple<Types...>>::valuetype&& get(simpletuple<Types...>&& t)
	{
		using type = typename simpletuple_element<index, simpletuple<Types...>>::thistype;
		using vtype = typename simpletuple_element<index, simpletuple<Types...>>::valuetype&&;
		return std::forward<vtype>(((type&)t).value);
	}

	template<class...Types>
	auto make_tuple(Types&& ...t)
	{
		return simpletuple<Types...>(std::forward<Types>(t)...);
	}

}
#endif // !ADRIVAL_SIMPLE_TUPLE_H