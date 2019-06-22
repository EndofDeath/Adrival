
#include "taskpool.h"
#include "cansteal_queue.h"
#include "thread_guards.h"

namespace adrival {

static thread_local cansteal_queue<function_wrapper>* local_work_queue = nullptr;
static thread_local unsigned local_index = 0;

class taskpool::impl
{
	using task = function_wrapper;
	using taskqueue = cansteal_queue<task>;
public:
	impl()
		:done_(false)
		,index_()
	{
		unsigned const thread_count = std::thread::hardware_concurrency();
		try
		{
			queues_.create(thread_count);
			threads_.create(thread_count);
			auto p = threads_.begin();
			for (unsigned i = 0; i < thread_count; ++i)
			{
				p[i] = std::thread(&impl::worker_thread, this, i);
			}
		}
		catch (...)
		{
			done_ = true;
			throw;
		}
	}
	~impl()
	{
		done_ = true;
	}
	void submit(task&& t)
	{
		if (local_work_queue)
		{
			local_work_queue->push(std::move(t));
		}
		else
		{
			queues_[index_++%queues_.size()]->push(std::move(t));
		}
	}
	bool try_run_task()
	{
		task t;
		if (steal_from_other_queue(t) ||
			pop_from_other_queue(t))
		{
			t();
			return true;
		}
		return false;
	}
private:
	class queue_guards
	{
	public:
		queue_guards():queues_(nullptr),count_(){}
		~queue_guards()
		{
			if (queues_)
				delete[] queues_;
			queues_ = nullptr;
		}
		taskqueue* operator[](unsigned index) const
		{
			return &queues_[index];
		}
		bool create(unsigned int count)
		{
			if (0 == count)
				return false;
			count_ = count;
			queues_ = new taskqueue[count];
			return true;
		}
		unsigned int size() const
		{
			return count_;
		}
		taskqueue* queues_;
		unsigned int count_;
	};
	std::atomic_bool done_;
	queue_guards queues_;
	thread_guards threads_;
	std::atomic_int index_;

	void worker_thread(unsigned my_index_)
	{
		local_index = my_index_;
		local_work_queue = queues_[local_index];
		while (!done_)
		{
			run_pending_task();
		}
	}
	void run_pending_task()
	{
		task t;
		if (pop_from_local_queue(t) ||
			steal_from_other_queue(t)
			)
		{
			t();
		}
		else
		{
			std::this_thread::yield();
		}
	}
	bool pop_from_local_queue(task& t)
	{
		return local_work_queue && local_work_queue->try_pop(t);
	}
	bool steal_from_other_queue(task& t)
	{
		for (unsigned i = 0; i < queues_.size(); ++i)
		{
			unsigned const index = (local_index + i + 1) % queues_.size();//从自己的下一个序号开始
			if (queues_[index]->try_steal(t))
			{
				return true;
			}
		}
		return false;
	}
	bool pop_from_other_queue(task& t)
	{
		for (unsigned i = 0; i < queues_.size(); ++i)
		{
			if (queues_[i]->try_pop(t))
			{
				return true;
			}
		}
		return false;
	}
};

taskpool::taskpool()
	:pimp_(new impl)
{
}

taskpool::~taskpool()
{
	if(pimp_)
		delete pimp_;
	pimp_ = nullptr;
}

bool taskpool::try_run_task()
{
	return pimp_->try_run_task();
}

void taskpool::submit_impl(function_wrapper && f)
{
	pimp_->submit(std::move(f));
}

}