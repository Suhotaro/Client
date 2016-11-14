#include <vector>

#include "job.h"


void PrimeNumbersJob::calculate() {
	if (l > h)
		return;

	printf("PRIMEJOB: start %d %d\n", l, h);

	/* Eratosthenes's algorithm with a minor optimization which gives us:
	* time : O(log(log(n)))
	* memory: O(n)
	*
	* TODO: decrease memory consumption to n/2
	* TODO: yet not optimized to calculte numbers "from  to "to", it calculate prime
	* numbers from 2 to "to"
	*/
	for (int i = 0; i < h; i++)
		v.push_back(1);

	v[0] = 0;
	v[1] = 0;

	for (unsigned int current_number = 2; current_number < v.size(); current_number++)
	{
		if (1 == v[current_number])
		{
			int power_two = current_number*current_number;

			for (size_t not_prime = power_two; not_prime < v.size(); not_prime += current_number)
				v[not_prime] = 0;
		}
	}
}

void PrimeNumbersJob::collect_data(Buffer &buffer) {
	for (int i = l; i < h; i++)
		if (1 == v[i])
			buffer.add_back(i);
}


std::vector<std::shared_ptr<Job>> PrimeNumbersJob::divide_job()
{
	std::vector<std::shared_ptr<Job>> jobs;

	if (h - l <= RANGE)
	{
		printf("DIVIDE JOB\n");

		jobs.push_back(std::shared_ptr<Job>(new PrimeNumbersJob(l, h)));
		return jobs;
	}

	int chunk = (h-l) % RANGE;

	for (int i = l; i <= h; i+= RANGE)
	{
		if (i + RANGE >= h)
		{
			printf("DIVIDE JOB:%d %d\n", i, i + chunk);
			jobs.push_back(std::shared_ptr<Job>(new PrimeNumbersJob(i, i + chunk)));
		}		
		else
		{
			printf("DIVIDE JOB:%d %d\n", i, i + RANGE);
			jobs.push_back(std::shared_ptr<Job>(new PrimeNumbersJob(i, i + RANGE)));
		}			
	}

	return jobs;
}
