#include "../include/finite_well.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <mpi.h>

static constexpr auto h = 1;
static constexpr auto pi = 3.14159265359;
static constexpr auto s = 2 * 1.0 / pow (h / (2 * pi), 2.0);
void plot (void);
auto finite_well::second_derivative (long double V) const -> long double
{
	return s * (V - _E) * _psi;
}

void finite_well::slope (long double V)
{
	_slope = _slope + this -> second_derivative (V);
}

auto finite_well::start_front (long double slope_,
							   long double psi) -> long double
{
	_slope = slope_;
	_psi = psi;
	long double x = 0.0;
	std::ofstream ofs;
	ofs.open ("test_front.txt", std::ofstream::out | std::ofstream::app);
	while (x <= _a) {
		_psi = _psi + _slope * _dx;
// 		write (x, _psi);
		{
			ofs << x << "," << _psi << std::endl;
		}
		slope (-1e-4);
		x += _dx;
	}
}

auto finite_well::start_back (long double slope_,
							  long double psi) -> long double
{
	_slope = slope_;
	_psi = psi;
	std::ofstream ofs;
	ofs.open ("test_back.txt", std::ofstream::out | std::ofstream::app);
	long double x = 3.0;
	_dx *= -1;
	while (x > _a) {
		_psi = _psi + _slope * _dx;
// 		write (x, _psi);
		{
			ofs << x << "," << _psi << std::endl;
		}
		slope (0.0);
		x += _dx;
	}
}
void finite_well::write (long double x, long double psi) const
{
	std::ofstream ofs;
	ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);
	ofs << x << "," << psi << std::endl;
}

void plot (void)
{
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	std::string commandsForGnuplot[] = {"set datafile separator \",\"", "y = 0", "plot y"};
	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[0].c_str());
	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[1].c_str());
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // 

int main (int argc, char **argv) 
{
	int rank_;
	finite_well obj;
	try {
		auto ec = MPI_Init (&argc, &argv);
		if (ec != MPI_SUCCESS) throw ec;
		std::cout << "Program Started" << std::endl;
	}
	
	catch (int ec) {
		MPI_Abort (MPI_COMM_WORLD, ec);
		std::cout << "MPI_Init Failed with error code : " << ec << std::endl
				  << "Aborting!" << std::endl;
	}
	
	MPI_Comm_rank (MPI_COMM_WORLD, &rank_);
	std::cerr << rank_ << std::endl;
	
	long double ratio;
	if (rank_ == 0)
	{
		obj.start_front (1e-1, 0.0);
		MPI_Recv(&ratio, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	else 
		if (rank_ == 1)
			obj.start_back (1e-10, 1e-3);
	
	else { 
		MPI_Finalize();
		return 0;
	}
	
	MPI_Finalize();
	return 0;
}
