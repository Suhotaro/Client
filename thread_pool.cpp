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
	std::vector<int> v;

	/* TODO: check low and high values */
	for(int i = 0; i < high; i++)
		v.push_back(1);

	v[0] = 0;
	v[1] = 0;

	/* Eratosthenes's algorithm with a minor optimization which gives us:
	 * time : loglogn
	 * memory: n
	 * 
	 * TODO: decrease memory consumption to n/2
	 */
	for(int i = 2; i < v.size(); i++)
		if (1 == v[i])
			for(int j = i*i; j < v.size(); j+=i)
				v[j] = 0;
	
	/* Write result to a buffer */
    for(int i = low; i < high; i++)
        if (1 == v[i])
        	buffers[buffer_idx].add_back(i);
	
	std::cout << "JOB: idx:" << buffer_idx <<  std::endl;
	
	buffers[buffer_idx].set_unused();
	all_buffers_captured.notify_one();
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
	while((free_buffer_idx = are_there_free_buffers()) < 0)
		all_buffers_captured.wait(lock);
	
	/* If there are more then "num_threads" threads in the vector remove first one */
	if (num_threads < jobs.size())
		jobs.erase(jobs.begin());
		
	/* Next job will fill free_buffer_idx buffer with prime numbers */
	buffers[free_buffer_idx].set_used();
	printf("free_buffer_idx:%d\n", free_buffer_idx);
	
	/* Start the job */
	jobs.push_back(std::thread(&ThreadPool::calculate_prime_numbers, this, free_buffer_idx, low, high));
	
	/* Detach job from main thread since we are not going to locked untill
	 * the job is finished */
	/* XXX: not sure if it is nice solution */
	jobs.back().detach();
}

void ThreadPool::show()
{
	for (int i = 0; i < num_threads; i++)
	{
		printf("buffer:%d\n", i+1);
		while(!buffers[i].empty())
			printf(" %d", buffers[i].get_front());
		printf("\n\n");
	}
		
		
			
}
