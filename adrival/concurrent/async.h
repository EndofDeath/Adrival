#ifndef ADRIVAL_ASYNC_H
#define ADRIVAL_ASYNC_H

#include <future>

namespace adrival {

	template<typename Fn, typename...Args>
	std::future<typename std::result_of<Fn && (Args&&...)>::type> async(Fn&& fn, Args&&... args)
	{
		std::packaged_task<typename std::result_of<Fn && (Args&&...)>::type()> task(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
		auto ret = task.get_future();
		std::thread t(std::move(task));
		t.detach();
		return ret;
	}

}
#endif // !ADRIVAL_ASYNC_H