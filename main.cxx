#include <hpc/library/include/fibonacci.h>
#include <hpc/utility/include/utility.h>

#include <iostream>
#include <chrono>
#include <functional>
#include "mpi.h"


std::uint32_t T;
std::uint32_t n [100];
void ass1 (void)
{
	for (std::uint32_t i = 0; i < T; ++i)
		std::cerr << fibonacci_iter (n[i]) <<  std::endl;
}

int main (void)
{
	std::cin >> T;
	for (std::uint32_t i = 0; i < T; ++i) std::cin >> n[i];

	auto time =
		exec_time (
		[]{
			for (std::uint32_t i = 0; i < T; ++i)
			std::cerr << fibonacci_iter (n[i]) <<  std::endl; 
			});

	std::cerr << time << std::endl;
}
