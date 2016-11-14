#if defined(WIN32) || defined(_WIN32)
#include "stdafx.h"
#endif

#include <thread>
#include <queue>
#include <mutex>

#include "buffer.h"

void Buffer::set_used()
{
	std::lock_guard<std::mutex> guard(used_mutex);
	used = true;
}

void Buffer::set_unused()
{
	std::lock_guard<std::mutex> guard(used_mutex);
	used = false;
}

bool Buffer::is_used()
{
	return used;
}

void Buffer::add_back(int val)
{
	std::lock_guard<std::mutex> guard(queue_mutex);
	queue.push_back(val);
}

int Buffer::get_front()
{
	int val;
	
	std::lock_guard<std::mutex> guard(queue_mutex);
	val = queue.front();
	queue.pop_front();

	return val;
}

bool Buffer::is_empty()
{
	std::lock_guard<std::mutex> guard(queue_mutex);
	return queue.empty();
}
