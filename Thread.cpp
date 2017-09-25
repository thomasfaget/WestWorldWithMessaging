#include "Thread.h"

Thread::Thread()
{
}

void Thread::start() {
	myThread = new std::thread(&Thread::run, this);
}

void Thread::join() {
	this->myThread->join();
}

Thread::~Thread()
{
}
