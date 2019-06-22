#ifndef FUTURE_TASK_H
#define FUTURE_TASK_H

#include <future>
#include "async.h"
#include "../utility/simpletuple.h"
#include "../utility/indexsequence.h"

namespace adrival{

template<class T>
class futuretask
{
public:
	futuretask() {}
	template<typename Fn, typename...Args>
	futuretask(Fn&& fn, Args&&... args)
	{
		future_ = adrival::async(fn, args...);
	}
	futuretask(futuretask<T>&& other)
	{
		future_ = std::move(other.future_);
	}
	futuretask(std::future<T>&&other)
	{
		future_ = std::move(other);
	}
	futuretask& operator=(futuretask<T>&&other)
	{
		future_ = std::move(other.future_);
		return *this;
	}
	futuretask& operator=(std::future<T>&&other)
	{
		future_ = std::move(other);
		return *this;
	}
	futuretask(const futuretask<T>&) = delete;
	futuretask& operator=(const futuretask<T>&) = delete;
	template<typename Fn>
	auto then(Fn&& fn)
	{
		auto shared = future_.share();
		return futuretask<typename std::result_of<Fn(T)>::type>([shared, fn] {
			return std::async(fn, shared.get()).share().get();
		});
	}
	template<typename Fn, typename...Fns>
	auto thenschedule(Fn&& fn, Fns&&...fns)
	{
		return then(fn).thenschedule(fns...);
	}
	template<typename Fn>
	auto thenschedule(Fn&& fn)
	{
		return then(fn);
	}
	void wait()
	{
		future_.wait();
	}
	T get()
	{
		return future_.get();
	}
	auto share()
	{
		return future_.share();
	}
private:
	std::future<T> future_;
};

template<>
class futuretask<void>
{
public:
	futuretask() {}
	template<typename Fn, typename...Args>
	futuretask(Fn&& fn, Args&&... args)
	{
		future_ = async(fn, args...);
	}
	futuretask(futuretask<void>&& other) noexcept
	{
		future_ = std::move(other.future_);
	}
	futuretask(std::future<void>&&other)
	{
		future_ = std::move(other);
	}
	futuretask& operator=(futuretask<void>&&other) noexcept
	{
		future_ = std::move(other.future_);
		return *this;
	}
	futuretask& operator=(std::future<void>&&other)
	{
		future_ = std::move(other);
		return *this;
	}
	futuretask(const futuretask<void>&) = delete;
	futuretask& operator=(const futuretask<void>&) = delete;
	template<typename Fn>
	auto then(Fn&& fn)
	{
		auto shared = future_.share();
		return futuretask<typename std::result_of<Fn()>::type>([shared, fn] {
			shared.get();
			return std::async(fn).share().get();
		});
	}
	template<typename Fn,typename...Fns>
	auto thenschedule(Fn&& fn, Fns&&...fns)
	{
		return then(fn).thenschedule(fns...);
	}
	template<typename Fn>
	auto thenschedule(Fn&& fn)
	{
		return then(fn);
	}
	void wait()
	{
		future_.wait();
	}
	void get()
	{
		if(!future_.valid())
			future_.get();
	}
	auto share()
	{
		return future_.share();
	}
private:
	std::future<void> future_;
};

template<typename Fn, typename T, size_t... _Idxs>
auto execute(Fn&& fn, T& _Tup, indexsequence<_Idxs...>)
{
	return fn(get< _Idxs>(_Tup)...);
}

template<class...Arg>
class futuretask<simpletuple<Arg...>>
{
public:
	futuretask() {}
	template<typename Fn, typename...Args>
	futuretask(Fn&& fn, Args&&... args)
	{
		future_ = adrival::async(std::forward<Fn>(fn), std::forward<Args>(args)...);
	}
	futuretask(futuretask<simpletuple<Arg...>>&& other)
	{
		future_ = std::move(other.future_);
	}
	futuretask(std::future<simpletuple<Arg...>>&&other)
	{
		future_ = std::move(other);
	}
	futuretask& operator=(futuretask<simpletuple<Arg...>>&&other)
	{
		future_ = std::move(other.future_);
		return *this;
	}
	futuretask& operator=(std::future<simpletuple<Arg...>>&&other)
	{
		future_ = std::move(other);
		return *this;
	}
	futuretask(const futuretask<simpletuple<Arg...>>&) = delete;
	futuretask& operator=(const futuretask<simpletuple<Arg...>>&) = delete;
	template<typename Fn>
	auto then(Fn&& fn)
	{
		auto shared = future_.share();
		return futuretask<typename std::result_of<Fn(Arg...)>::type>([shared, fn] {
			return std::async([shared, fn] {
				return execute(fn, shared.get(), indexsequence_for<Arg...>());
			}).share().get();
		});
	}
	template<typename Fn, typename...Fns>
	auto thenschedule(Fn&& fn, Fns&&...fns)
	{
		return then(fn).thenschedule(fns...);
	}
	template<typename Fn>
	auto thenschedule(Fn&& fn)
	{
		return then(fn);
	}
	void wait()
	{
		future_.wait();
	}
	simpletuple<Arg...> get()
	{
		return future_.get();
	}
	auto share()
	{
		return future_.share();
	}
private:
	std::future<simpletuple<Arg...>> future_;
};

template<typename Fn, typename...Args>
auto make_futuretask(Fn&& fn, Args&&... args)
{
	using rettype = typename std::result_of<Fn && (Args&&...)>::type;
	return futuretask<rettype>(fn, args...);
}

template<typename...Arg>
auto when_all(futuretask<Arg>&...tasks)
{
	auto fp = adrival::async([](std::shared_future<Arg>...futures) {
		return make_tuple(std::move(std::decay_t<Arg>(futures.get()))...);
	}, tasks.share()...);
	return futuretask<simpletuple<Arg...>>(std::move(fp));
}

}
#endif // !FUTURE_TASK_H