#ifndef ADRIVAL_TASKPOOL_H
#define ADRIVAL_TASKPOOL_H

#include <future>
#include "../utility/funtion_wrapper.h"
namespace adrival {

class taskpool
{
public:
	taskpool();
	~taskpool();
	taskpool(const taskpool&) = delete;
	taskpool(taskpool&&) = delete;
	taskpool& operator=(const taskpool&) = delete;
	taskpool& operator=(taskpool&&) = delete;
	template<typename FunctionType, typename...Args>
	std::future<typename std::result_of<FunctionType && (Args&&...)>::type> submitpackage(FunctionType&& f, Args&&... args)
	{
		typedef typename std::result_of<FunctionType && (Args&&...)>::type result_type;
		std::packaged_task<result_type()> task(make_wrapper(std::forward<FunctionType>(f), std::forward<Args>(args)...));
		std::future<result_type> res(task.get_future());
		submit_impl(std::move(task));
		return res;
	}
	template<typename FunctionType, typename...Args>
	void submit(FunctionType&& f, Args&&... args)
	{
		function_wrapper task(make_wrapper(std::forward<FunctionType>(f), std::forward<Args>(args)...));
		submit_impl(std::move(task));
	}
	bool try_run_task();
private:
	void submit_impl(function_wrapper&& f);
	class impl;
	impl* pimp_;
};

}
#endif // !ADRIVAL_TASKPOOL_H