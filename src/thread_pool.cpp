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

	printf("POOL: num of workers threads %d\n", thread_count);

	for (unsigned int i = 0; i < thread_count; i++)
	{
		works_threads.push_back(std::thread(&ThreadPool::worker_thread, this));
		buffers.insert( std::pair<std::thread::id, Buffer>(works_threads[i].get_id(), Buffer()));
		statistic.insert( std::pair<std::thread::id, int>(works_threads[i].get_id(), 0));
	}

	/* TODO: it has to be effciently calculated */
	//threads_per_cpu = 20;

	puller = std::unique_ptr<std::thread> (new std::thread(std::thread(&ThreadPool::pull, this)));
	puller_run = true;
}

ThreadPool::~ThreadPool()
{
	works_run = false;

	/* Wait until all threads are stoped */
	for (unsigned int i = 0; i < works_threads.size(); i++)
		if (works_threads[i].joinable())
			works_threads[i].join();

	puller_run = false;

	if (puller->joinable())
		puller->join();

	statistic_show();
}

void ThreadPool::worker_thread()
{
	printf("POOL: start work thread\n");

	while(works_run)
	{
		if (false == do_jobs())
			std::this_thread::yield();
	}

	/* If there are yet not finished jobs remaining process them all */
	while(true)
	{
		works_queue_mutex.lock();

		if (works_queue.empty())
		{
			works_queue_mutex.unlock();
			printf("POOL: stop work thread\n");

			return;
		}

		Job job = works_queue.front();
		works_queue.pop_front();

		works_queue_mutex.unlock();

		printf("POOL: process job\n");

		Buffer &buffer = buffers[std::this_thread::get_id()];
		job(buffer);
	}
}

bool ThreadPool::do_jobs()
{
	works_queue_mutex.lock();

	if (works_queue.empty())
	{
		works_queue_mutex.unlock();
		return false;
	}

	Job job = works_queue.front();
	works_queue.pop_front();

	int low = job.get_low();
	int high = job.get_high();
	int range = high - low;

	Buffer &buffer = buffers[std::this_thread::get_id()];

	if (range <= max_range)
	{
		statistic_do(1);

		works_queue_mutex.unlock();

		printf("POOL: no paralelize case @@@@@@@@@");

		job(buffer);
		return true;
	}

	printf("POOL: paralelize case #########\n");

	int rest = range % max_range;
	int chanks =  range / max_range;
	int cut = high - ((chanks - threads_per_cpu) * max_range) - rest;

	std::vector<std::thread> sub_workers_threads;

	if (chanks > threads_per_cpu)
	{
		printf("POOL: split 1111111111\n");

		/* TODO: It might be possible that the job could be skipped on destroing the pool */
		works_queue.push_back(Job(cut, high - rest));
		works_queue_mutex.unlock();

		statistic_do(1);

		printf("POOL: split 1111111111 %dx%d\n", cut, high - rest);

		for (int i = low; i < cut; i+=max_range)
		{
			if (i+max_range >= cut)
				sub_workers_threads.push_back(std::thread(Job(i, i+max_range+rest), std::ref(buffer)));
			else
				sub_workers_threads.push_back(std::thread(Job(i, i+max_range), std::ref(buffer)));
		}
	}
	else
	{
		works_queue_mutex.unlock();

		printf("POOL: split 22222222222");

		for (int i = low; i < high - rest; i+=max_range)
		{
			statistic_do(1);

			if (i+max_range >= high - rest)
				sub_workers_threads.push_back(std::thread(Job(i, i+max_range+rest), std::ref(buffer)));
			else
				sub_workers_threads.push_back(std::thread(Job(i, i+max_range), std::ref(buffer)));
		}
	}

	for (unsigned int i = 0; i < sub_workers_threads.size(); i++)
		sub_workers_threads[i].join();

	return true;
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

void ThreadPool::collect_data_and_send()
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

void ThreadPool::pull()
{
	printf("POOL: start pull +++++++>>\n");

	while (puller_run)
		collect_data_and_send();

	/* After that point all worker threads are already stoped, thus
	 * send data that left in our buffers
	 */
	collect_data_and_send();

	printf("POOL: stop pull <<+++++++\n");
}

void ThreadPool::send(std::vector<int> &data)
{
	FakeTCP tcp("127.0.0.1:80");
	tcp.set_data(data);
	tcp.send();
}

void ThreadPool::statistic_do(int num)
{
	statistic_mutex.lock();

	int tmp = statistic[std::this_thread::get_id()];
	statistic.erase(std::this_thread::get_id());
	statistic.insert( std::pair<std::thread::id, int>(std::this_thread::get_id(), tmp+num));

	/* Doing it just to better remember unique_lock feture */
	statistic_mutex.unlock();
}

void ThreadPool::statistic_show()
{
	printf("-----------------------------\n\n");
	printf("Statistic:\n");
	int j = 1;
	for (auto i = statistic.begin(); i != statistic.end(); i++)
	{
		printf("  thread : %d\n", i->second);
		j++;
	}
	printf( "-----------------------------\n\n");
}
