#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "buffer.h"

class ThreadPool
{
private:
	Buffer *buffers;
	int num_threads = 0;
	
public:	
	ThreadPool(int num_threads);
	~ThreadPool();
	
	void start_job(int low, int high);
};

#endif //__THREAD_POOL_H__
