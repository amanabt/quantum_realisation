#include "../include/utility.h"
#include <cstdio>

using namespace utils;

void swap (long long int& a,
		   long long int& b)
{
	auto temp = a;
	a = b;
	b = temp;
}

// // // // // // // // // // // // // // // // // // // // // // // // 

bool pipe (const std::string cmd)
{
	try {
		if (!popen (cmd.c_str(), "w"))
			throw false;
	}
	
	catch (bool error){
		std::cout << "Piping Command " << cmd << " failed"
		<< "With error code : " << error << std::endl;
		return false;
	}
	
	return true;
}

// // // // // // // // // // // // // // // // // // // // // // // // // 

void plot (void)
{
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	std::string commandsForGnuplot[] =
	{ "set set datafile separator \",\"",
	  "plot \"test_front.txt\" using 1:2 with l; replot \"test_back.txt\" using 1:2 with l"
	};
	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[0].c_str());
	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[1].c_str());
}

// // // // // // // // // // // // // // // // // // // // // // // // // 
