#ifndef ADRIVAL_CONCEPT_H
#define ADRIVAL_CONCEPT_H

#include <type_traits>
#include <functional>
#include "memberfun_traits.h"
#include "Optional.h"

#define DECLARE_CONCEPT(name)\
template<typename R = void, typename ...Arg>\
class concept_##name\
{\
public:\
	concept_##name() = default;\
	template<class T, class = std::enable_if<!std::is_same<typename std::decay<T>::type, concept_##name<R, Arg...>>::value>::type>\
	concept_##name(T&& v)\
	{\
		using classtype = typename std::decay<T>::type;\
		static_assert(std::is_class<classtype>::value, "the type isn't class");\
		static_assert(has_memfn_##name<classtype, R, Arg...>::value, "the class is no matching member function");\
		_fun = [&v](Arg... arg) {v.name(arg...);};\
	}\
	concept_##name(const concept_##name& rhs)\
	{\
		_fun = rhs._fun;\
	}\
	concept_##name(concept_##name&& rhs)\
	{\
		_fun = std::move(rhs._fun);\
	}\
	concept_##name& operator=(concept_##name<R, Arg...>&& rhs)\
	{\
		_fun = std::move(rhs._fun);\
		return *this;\
	}\
	template<class T, class = std::enable_if<!std::is_same<typename std::decay<T>::type, concept_##name<R, Arg...>>::value>::type>\
	concept_##name& operator=(T&& v)\
	{\
		*this = concept_##name(std::forward<T>(v));\
		return *this;\
	}\
	concept_##name& operator = (const concept_##name<R, Arg...>& rhs)\
	{\
		_fun = rhs._fun;\
		return *this;\
	}\
	adrival::optional<R> name(Arg... arg)\
	{\
		if (_fun)\
			return _fun(arg...);\
		else\
			return {};\
	}\
private:\
	std::function<R(Arg...)> _fun;\
};\
template<typename ...Arg>\
class concept_##name<void, Arg...>\
{\
public:\
	concept_##name() = default;\
	template<class T, class = std::enable_if<!std::is_same<typename std::decay<T>::type, concept_##name<void, Arg...>>::value>::type>\
	concept_##name(T&& v)\
	{\
		using classtype = typename std::decay<T>::type;\
		static_assert(std::is_class<classtype>::value, "the type is not a class");\
		static_assert(has_memfn_##name<classtype, void, Arg...>::value, "In the class,there are no matching member function");\
		_fun = [&v]( Arg... arg) {v.name(arg...);};\
	}\
	concept_##name(const concept_##name& rhs)\
	{\
		_fun = rhs._fun;\
	}\
	concept_##name(concept_##name&& rhs)\
	{\
		_fun = std::move(rhs._fun);\
	}\
	concept_##name& operator=(concept_##name<void, Arg...>&& rhs)\
	{\
		_fun = std::move(rhs._fun);\
		return *this;\
	}\
	template<class T, class = std::enable_if<!std::is_same<typename std::decay<T>::type, concept_##name<void, Arg...>>::value>::type>\
	concept_##name& operator=(T&& v)\
	{\
		*this = concept_##name(std::forward<T>(v));\
		return *this;\
	}\
	concept_##name& operator = (const concept_##name<void, Arg...>& rhs)\
	{\
		_fun = rhs._fun;\
		return *this;\
	}\
	void name(Arg... arg)\
	{\
		if (_fun)\
			_fun(arg...);\
	}\
private:\
	std::function<void(Arg...)> _fun;\
};\

#define IMPLEMENT_CONCEPT(name)\
	HAS_MEMFN(name)\
	DECLARE_CONCEPT(name)\

#endif // !ADRIVAL_CONCEPT_H