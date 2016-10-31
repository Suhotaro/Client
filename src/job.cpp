#include <vector>

#include "job.h"

void Job::calculate_pime_numbers(Buffer &buffer, int from, int to)
{
	if (to < from)
		return;
	
	/* Eratosthenes's algorithm with a minor optimization which gives us:
	 * time : O(log(log(n)))
	 * memory: O(n)
	 * 
	 * TODO: decrease memory consumption to n/2
	 * TODO: yet not optimized to calculte numbers "from  to "to", it calculate prime
	 * numbers from 2 to "to"
	 */
	for(int i = 0; i < to; i++)
		v.push_back(1);

	v[0] = 0;
	v[1] = 0;
	
	for(unsigned int current_number = 2; current_number < v.size(); current_number++)
	{
		if (1 == v[current_number])
		{
			int power_two = current_number*current_number;

			for(size_t not_prime = power_two; not_prime < v.size(); not_prime+=current_number)
				v[not_prime] = 0;
		}
	}

	buffer.set_used();
	/* Write result to a buffer */
	for(int i = from; i < to; i++)
		if (1 == v[i])
			buffer.add_back(i);
			
	buffer.set_unused();
}

void Job::operator() (Buffer &buffer, int from, int to)
{
	calculate_pime_numbers(buffer, from, to);
}

void Job::operator() (Buffer &buffer)
{
	printf("JOB: do job:%d %d\n", low, high);
	
	calculate_pime_numbers(buffer, low, high);
	
	printf("JOB: done job\n");
}

