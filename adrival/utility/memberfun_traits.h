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

#define has_men_fn(name)\
template<typename T,typename R = void,typename ...Arg>\
class has_mem_fn_##name\
{\
	struct twochar\
	{\
		char m[2];\
	};\
	template<typename T>\
	static twochar trait_##name(...)\
	{\
		return 0;\
	}\
	template<typename T>\
	static char trait_##name(decltype(adrival::mem_fn_pointer<T,R,Arg...>(&T::name)))\
	{\
		return 0;\
	}\
	template<typename T>\
	static twochar trait_##name_noarg(...)\
	{\
		return 0;\
	}\
	template<typename T>\
	static char trait_##name_noarg(decltype(adrival::mem_fn_pointer_noarg<T,R>(&T::name)))\
	{\
		return 0;\
	}\
public:\
	static const bool value = 0 < sizeof...(Arg) ? sizeof(decltype(trait_##name<T>(0))) == 1:sizeof(decltype(trait_##name_noarg<T>(0))) == 1 ;\
};

#endif // !ADRIVAL_MENBERFUN_TRAITS_H