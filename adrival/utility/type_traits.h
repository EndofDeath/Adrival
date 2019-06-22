#pragma once

template<class T>
class IsClass
{
	template<class T>
	static char trait(char T::*)
	{
		return 0;
	}
	struct twochar
	{
		char mc[2];
	};
	template<class T>
	static twochar trait(...)
	{
		return 0;
	}
public:
	static const bool value = sizeof(decltype(trait<T>(0))) == 1;
};