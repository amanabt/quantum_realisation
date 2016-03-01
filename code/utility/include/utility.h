#ifndef __UTILITY__
#define __UTILITY__

#include <chrono>

void swap (long long int& a,
		   long long int& b);

template <typename Func>
double exec_time (const Func& foo)
{
	std::chrono::high_resolution_clock::time_point start =
		std::chrono::high_resolution_clock::now();

	foo();
	std::chrono::high_resolution_clock::time_point end =
		std::chrono::high_resolution_clock::now();
		
	return (std::chrono::duration_cast<std::chrono::duration<double>>(end - start)).count();
}

#endif