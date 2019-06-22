#ifndef ADRIVAL_ITHREAD_H
#define ADRIVAL_ITHREAD_H

#include <thread>

namespace adrival{

class Ithread
{
public:
	Ithread();
	virtual ~Ithread();
	void start();
	void join();
	virtual void stop() = 0;
protected:
	virtual void run() = 0;
private:
	void threadfun();
	std::shared_ptr<std::thread> thread_;
};

}
#endif // !ADRIVAL_ITHREAD_H