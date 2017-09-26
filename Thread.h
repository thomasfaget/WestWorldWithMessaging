#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <mutex>

class Thread
{
public:
	Thread();
	~Thread();
	void start();
	void join();

	void lock();
	void unlock();

protected:
	virtual void run() = 0;
	int id;
	std::thread* myThread;
	std::mutex mutex;
};

#endif

