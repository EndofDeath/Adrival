#ifndef ADRIVAL_FUNCTION_TRAITS_H
#define ADRIVAL_FUNCTION_TRAITS_H
#pragma once

#include <functional>

namespace adrival{

template<typename T>
struct function_traits;

template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{
public:
	enum { number_args = sizeof...(Args) };
	typedef Ret(function_type)(Args...);
	using return_type = Ret;
	using std_function_type = std::function<Ret(Args...)>;
	using function_reference = Ret(&)(Args...);
	using function_pointer = Ret(*)(Args...);

	template<size_t I>
	struct args
	{
		static_assert(I < number_args, "no more Arguments in function");
		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
	};
};

template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> :function_traits < Ret(Args...) > {};

template<typename Ret, typename... Args>
struct function_traits<Ret(&)(Args...)> :function_traits < Ret(Args...) > {};

template<typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>> :function_traits < Ret(Args...) > {};


#define FUNCTION_TRAITS(...) \
    template <typename ReturnType, typename ClassType, typename... Args>\
    struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{\
		using class_type = ClassType;\
		using memfun_pointer = ReturnType(ClassType::*)(Args...) __VA_ARGS__;\
}; \

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

template<typename FuncObject>
struct function_traits :function_traits < decltype(&FuncObject::operator()) > {};

}
#endif // !ADRIVAL_FUNCTION_TRAITS_H