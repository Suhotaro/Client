#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

#include "thread_pool.h"
#include "buffer.h"
#include "util.h"
#include "fake_tcp.h"

ThreadPool::ThreadPool() : works_run(true),
works_threads_joiner(works_threads), puller_joiner(puller)
{
	const unsigned int thread_count = std::thread::hardware_concurrency();

	printf("NUM: %d\n", thread_count);

	for (unsigned int i = 0; i < thread_count; i++)
	{
		works_threads.push_back(std::thread(&ThreadPool::worker_thread, this));
		buffers.insert( std::pair<std::thread::id, Buffer>(works_threads[i].get_id(), Buffer()));
	}

	puller = std::unique_ptr<std::thread> (new std::thread(std::thread(&ThreadPool::pull, this)));
	puller_run = true;
}

ThreadPool::~ThreadPool()
{
	/* XXX: as I understand during deleting an object its all fields destructores
	 * has to be called backwards to its fields constructores. It means the we
	 * will join to all work threads and after will join to puller thread. But
	 * I am not sure for that right now.
	 */
	works_run = false;
	puller_run = false;
}

void ThreadPool::worker_thread()
{
	printf("POOL: start work thread\n");

	while(works_run)
	{
		works_queue_mutex.lock();

		if (!works_queue.empty())
		{
			Job job = works_queue.front();
			works_queue.pop_front();

			works_queue_mutex.unlock();

			printf("POOL: process job\n");

			Buffer &buffer = buffers[std::this_thread::get_id()];
			job(buffer);

			/* paralelize jobs */
		}
		else
		{
			works_queue_mutex.unlock();
			std::this_thread::yield();
		}
	}

	printf("POOL: stop work thread\n");
}


/* public functions */
void ThreadPool::start_job(int low, int high)
{
	std::lock_guard<std::mutex> guard(works_queue_mutex);

	works_queue.push_back(Job(low, high));
}

void ThreadPool::show()
{
#if 0
	for (int i = 0; i < num_threads; i++)
	{
		printf("buffer:%d\n", i+1);
		while(!buffers[i].is_empty())
			printf(" %d", buffers[i].get_front());
		printf("\n\n");
	}			
#endif
}

void ThreadPool::pull()
{	
	printf("POOL: start pull +++++++>>\n");

	while (puller_run)
	{
		for(auto iterator = buffers.begin(); iterator != buffers.end(); iterator++)
		{
			std::vector<int> data;
			Buffer &buffer = iterator->second;

			/* Pull calculated prime numbers from a buffer */
			while (!buffer.is_empty())
				data.push_back(buffer.get_front());

			/* send data via fake tcp */
			if (!data.empty())
			{
				printf("POOL: send data ------>>\n");

				std::thread sender(&ThreadPool::send, this, std::ref(data));
				sender.join();
			}
		}
	}

	printf("POOL: stop pull <<+++++++\n");
}

void ThreadPool::send(std::vector<int> &data)
{
	FakeTCP tcp;
	tcp.init("127.0.0.1:80");
	tcp.send(data);
}


