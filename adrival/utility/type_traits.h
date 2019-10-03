#pragma once

namespace adrival{

	struct sfine
	{
	protected:
		using one = char;
		struct two
		{
			char c[2];
		};
	};

	template<class _Ty>
	_Ty&& declval() {};

template<class T>
class is_class:public sfine
{
	template<class T>
	static one trait(char T::*)
	{
		return 0;
	}
	template<class T>
	static two trait(...)
	{
		return 0;
	}
public:
	static constexpr bool value = sizeof(decltype(trait<T>(0))) == sizeof(one);
};

template<class T>
constexpr bool is_class_v = is_class<T>::value;

template<class F,class ...Args>
class is_invocable:public sfine
{
	template<class Fn,class ...args,class = decltype(declval<Fn>()(declval<args>()...))>
	static one trait(int){}

	template<class Fn, class ...args>
	static two trait(...) {}
public:
	static constexpr bool value = sizeof(decltype(trait<F&&,Args&&...>(0))) == sizeof(one);
};

template<class F, class ...Args>
constexpr bool is_invocable_v = is_invocable<F, Args...>::value;

}