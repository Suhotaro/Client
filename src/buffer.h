#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <deque>
#include <thread>
#include <mutex>

class Buffer
{
private:
	std::deque<int> queue;
	bool used = false;
	std::mutex mtx;
	
public:
	Buffer(): used(false) {}
	Buffer(bool use): used(use) {}
	~Buffer() {}
	
	// make it noncopyable
	Buffer(Buffer const&) = delete;
	Buffer& operator=(Buffer const&) = delete;

    // make it movable (thanks, gx_)
	Buffer( Buffer&&) = default;
	Buffer& operator=(Buffer&&) = default;

	void set_used();
	void set_unused();
	bool is_used();

	void add_back(int val);
	int get_front();
	bool is_empty();
};

#endif //__BUFFER_H__
