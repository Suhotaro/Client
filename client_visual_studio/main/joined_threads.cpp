#include "stdafx.h"

#include <thread>

#include "joined_threads.h"

JoinedThreads::JoinedThreads(std::vector<std::thread> &threads_) : threads (threads_)
{
}

JoinedThreads::~JoinedThreads()
{
	for (unsigned int i = 0; i < threads.size(); i++)
		if (threads[i].joinable())
			threads[i].join();
}

JoinedThread::JoinedThread(std::unique_ptr<std::thread> &thread_) : thread (thread_)
{
}

JoinedThread::~JoinedThread()
{
	if (thread->joinable())
		thread->join();
}