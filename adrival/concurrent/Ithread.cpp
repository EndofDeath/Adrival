
#include "Ithread.h"

namespace adrival {

Ithread::Ithread()
{
}

Ithread::~Ithread()
{
	join();
}

void Ithread::start()
{
	if (!thread_)
		thread_.reset(new std::thread(&Ithread::threadfun, this));
}

void Ithread::join()
{
	if (thread_ && thread_->joinable())
		thread_->join();
}

void Ithread::threadfun()
{
	run();
}

}