#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>

#include "thread_pool.h"
#include "buffer.h"
#include "util.h"

ThreadPool::ThreadPool(int num_threads) : num_threads(num_threads)
{
	buffers = new Buffer[num_threads];
	EXITIFTRUE(buffers == NULL, "allocate buffers fail");
}

ThreadPool::~ThreadPool()
{
	delete[] buffers;
}


/* private functions */
bool ThreadPool::are_there_free_buffers()
{
	bool used = false;
	
	for (int i = 0; i < num_threads; i++)
	{
		used = buffers[i].is_used();
		if (used == false)
			return i;
	}

	return -1;
}

void ThreadPool::calculate_prime_numbers(int buffer_idx, int low, int high)
{
	std::cout << "ThreadPool: JOB l:"
			  << low << " h:" << high << std::endl;
	
	buffers[buffer_idx].set_unused();
}

/* public functions */
void ThreadPool::start_job(int low, int high)
{
	int free_buffer_idx = -1;
	
	std::cout << "ThreadPool: job started l:"
			  << low << " h:" << high << std::endl;
	
	/* If there are free buffers exist start calculation of prime numbers thread
	 * for given range of numbers if there are no free buffers, wait untill one
	 * of already runned threads free one of captured buffers buffers */
	std::unique_lock<std::mutex> lock(mtx);
	while(-1 == (free_buffer_idx = are_there_free_buffers()))
		all_buffers_captured.wait(lock);
	
	/* If there are more then "num_threads" threads in the vector remove first one */
	if (num_threads < jobs.size())
		jobs.erase(jobs.begin());
		
	buffers[free_buffer_idx].set_used();
	jobs.push_back(std::thread(&ThreadPool::calculate_prime_numbers, this, free_buffer_idx, low, high));
	/* XXX: not sure if it is nice solution */
	jobs.back().detach();
}
