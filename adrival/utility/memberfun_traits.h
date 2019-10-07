#ifndef ADRIVAL_MENBERFUN_TRAITS_H
#define ADRIVAL_MENBERFUN_TRAITS_H

namespace adrival{
#define mem_fun_pointer(...)\
template< typename T,typename R = void>\
static auto mem_fn_pointer_noarg(R(T::*p)()__VA_ARGS__)->decltype(p)\
{\
	return p;\
}\
template<typename T, typename R = void, class ...Arg>\
static auto mem_fn_pointer(R(T::*p)(Arg...)__VA_ARGS__)->decltype(p)\
{\
	return p;\
}

mem_fun_pointer()
mem_fun_pointer(const)
mem_fun_pointer(volatile)
mem_fun_pointer(const volatile)

}

#define HAS_MEMFN(name)\
template<class T, class R=void,class...Arg>\
class has_memfn_##name {\
	static void trait_helper(R(T::*)(Arg...)) {}\
	template<typename C>\
	static auto trait(int)->decltype(trait_helper(&C::name), std::true_type{}) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};\
template<class T, class R=void,class...Arg>\
class has_memfn_##name##_c{ \
	static void trait_helper(R(T::*)(Arg...) const) {}\
	template<typename C>\
	static auto trait(int)->decltype(trait_helper(&C::name), std::true_type{}) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};\
template<class T, class R=void,class...Arg>\
class has_memfn_##name##_v{ \
	static void trait_helper(R(T::*)(Arg...) volatile) {}\
	template<typename C>\
	static auto trait(int)->decltype(trait_helper(&C::name), std::true_type{}) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};\
template<class T, class R=void,class...Arg>\
class has_memfn_##name##_cv{ \
	static void trait_helper(R(T::*)(Arg...) const volatile) {}\
	template<typename C>\
	static auto trait(int)->decltype(trait_helper(&C::name), std::true_type{}) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};

#define HAS_EXPLICIT_MEMFN(name)\
template<class T,class R=void,class ...Arg>\
class has_explicit_memfn_##name{\
	template <typename U, U> struct explicit_has;\
	template<typename C> static std::true_type trait(explicit_has <R(C::*)(Arg...), &C::name>*) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};\
template<class T,class R=void,class ...Arg>\
class has_explicit_memfn_##name##_c{\
	template <typename U, U> struct explicit_has;\
	template<typename C> static std::true_type trait(explicit_has <R(C::*)(Arg...) const, &C::name>*) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};\
template<class T,class R=void,class ...Arg>\
class has_explicit_memfn_##name##_v{\
	template <typename U, U> struct explicit_has;\
	template<typename C> static std::true_type trait(explicit_has <R(C::*)(Arg...) volatile, &C::name>*) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};\
template<class T,class R=void,class ...Arg>\
class has_explicit_memfn_##name##_cv{\
	template <typename U, U> struct explicit_has;\
	template<typename C> static std::true_type trait(explicit_has <R(C::*)(Arg...) const volatile, &C::name>*) {};\
	template<typename C> static std::false_type trait(...) {};\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};

#define HAS_CALLABLE_MEMFN(name)\
template<class T, class...Arg>\
class has_callable_memfn_##name{\
	template<typename U> static auto trait(int) -> decltype(std::declval<U>().name(std::declval<Args>()...), std::true_type{}) {}\
	template<typename U> static std::false_type trait(...) {}\
public:\
	static constexpr bool value = decltype(trait<T>(0))::value;\
};
#endif // !ADRIVAL_MENBERFUN_TRAITS_H