#ifndef ADRIVAL_FUNCTION_WRAPPER_H
#define ADRIVAL_FUNCTION_WRAPPER_H

#include <memory>

namespace adrival{

class function_wrapper
{
public:
	template<typename F>
	function_wrapper(F&& f) : pimp(new implT <F>(std::move(f))) {}
	function_wrapper() = default;
	function_wrapper(function_wrapper&& other) noexcept : pimp(std::move(other.pimp)) {}
	function_wrapper& operator=(function_wrapper&& other) noexcept
	{
		pimp = std::move(other.pimp);
		return *this;
	}
	void operator()()
	{
		if (pimp)
			pimp->invoke();
	}

	function_wrapper(const function_wrapper&) = delete;
	function_wrapper& operator=(const function_wrapper&) = delete;
private:
	struct implbase
	{
		virtual void invoke() = 0;
		virtual ~implbase() {}
	};
	std::unique_ptr<implbase> pimp;
	template<typename F>
	struct implT : implbase
	{
		F fn_;
		implT(F&& fn) : fn_(std::move(fn)) {}
		void invoke()
		{
			fn_();
		}
	};
};

#include <type_traits>

template<typename Fn, typename Class, typename ...Args>
auto make_wrapper_menfn(Fn&& fun, Class& obj, Args&&...args)
{
	return [&] {return (obj.*fun)(arg...); };
}

template<typename Fn, typename Class, typename ...Args>
auto make_wrapper_menfn(Fn&& fun, Class* obj, Args&&...args)
{
	return [&] {return (obj->*fun)(arg...); };
}

template<typename Fn, typename ...Args>
auto make_wrapper_impl(Fn&& fun, std::true_type, Args&&...args)
{
	return make_wrapper_menfn(std::forward<Fn>(fun), std::forward<Args>(args)...);
}

template<typename Fn, typename ...Args>
auto make_wrapper_impl(Fn&& fun, std::false_type, Args&&...args)
{
	return [&] {return fun(args...); };
}

template<typename Fn,typename ...Args>
auto make_wrapper(Fn&& fun, Args&&...args)
{
	return make_wrapper_impl(std::forward<Fn>(fun), std::is_member_function_pointer<Fn>(), std::forward<Args>(args)...);
}

}
#endif // !ADRIVAL_FUNCTION_WRAPPER_H