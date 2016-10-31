#ifndef __JOB_H__
#define __JOB_H__

#include <vector>
#include "buffer.h"

class Job
{
private:
	int low;
	int high;
	
	std::vector<int> v;

	void calculate_pime_numbers(Buffer &buffer, int from, int to);
	
public:
	Job () = delete;
	Job (int low_, int high_) : low(low_), high(high_) {}
		
	Job(const Job& job) : low(job.low), high(job.high) {}
	Job& operator=(const Job& job)
    {
		low = job.low;
		high = job.high;

    	return *this;
    }

	/* calculcates prime numbers from "low" to "high" and stores result in buffer */
	void operator() (Buffer &buffer);
	
	/* calculcates prime numbers from "from" to "to" and stores result in buffer */
	void operator() (Buffer &buffer, int from, int to);
};

#endif //__JOB_H__