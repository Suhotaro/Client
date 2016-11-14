#ifndef __JOB_H__
#define __JOB_H__

#include <vector>
#include "buffer.h"

class Job
{	
	virtual void calculate() = 0;

public:
	Job() {}
	virtual ~Job() {}

	void do_job() {

		printf("start do_hob\n");

		calculate();
	}

	virtual void collect_data(Buffer &buffer) = 0;
	virtual std::vector<std::shared_ptr<Job>> divide_job() = 0;
};

class PrimeNumbersJob : public Job
{
	enum {RANGE = 20};
	int range = RANGE;
	
	int l, h;
	std::vector<int> v;

	void calculate();

public:
	PrimeNumbersJob(int l_, int h_): l(l_), h(h_) {}
	~PrimeNumbersJob() {}
		
	void collect_data(Buffer &buffer);
	std::vector<std::shared_ptr<Job>> divide_job();
};

#endif //__JOB_H__
