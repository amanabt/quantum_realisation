#include "../include/finite_well.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <mpi.h>
#include <memory>
#include <unistd.h>

static constexpr auto h = 1;
static constexpr auto pi = 3.14159265359;
static constexpr auto s = 2 * 1.0 / pow (h / (2 * pi), 2.0);

void plot (void);

auto finite_well::second_derivative (double V) const -> double
{
	return s * (V - _E) * _psi;
}

// // // // // // // // // // // // // // // // // // // // // // // // 

void finite_well::slope (double V)
{
	_slope = _slope + this -> second_derivative (V);
}

// // // // // // // // // // // // // // // // // // // // // // // // 

auto finite_well::start_front (double slope_,
							   double psi) -> double
{
	remove("test_front.txt");
	_slope = slope_;
	_psi = psi;
	double x = 0.0;
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

	return _E;
}

// // // // // // // // // // // // // // // // // // // // // // // // 

auto finite_well::start_back (double slope_,
							  double psi) -> double
{
	remove ("test_back.txt");
	_slope = slope_;
	_psi = psi;
	std::ofstream ofs;
	ofs.open ("test_back.txt", std::ofstream::out | std::ofstream::app);
	double x = 2;
	if (_dx > 0.0) _dx = _dx * -1;
	while (x > _a) {
		_psi = _psi + _slope * _dx;
// 		write (x, _psi);
		{
			ofs << x << "," << _psi << std::endl;
		}
		slope (0.0);
		x += _dx;
	}

	return _E;
}

// // // // // // // // // // // // // // // // // // // // // // // // // 

void finite_well::write (double x, double psi) const
{
	std::ofstream ofs;
	ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);
	ofs << x << "," << psi << std::endl;
}

// // // // // // // // // // // // // // // // // // // // // // // // // 

namespace utils {
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
};// namespace utils

// // // // // // // // // // // // // // // // // // // // // // // // // // // // 

int main (int argc, char **argv) 
{
	int rank_;
	std::unique_ptr<finite_well> particle (new finite_well);
	double dE = -1e-12, E;
	double ratio;

// 	auto i = 100;
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
	
	if (rank_ == 0)
	{
		while (true){
			particle->start_front (1e-3, 0.0);
			MPI_Recv (&ratio, 1, MPI_LONG_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
// 			std::cerr << ratio << std::endl;
			E = particle->E() + dE;
			if (!particle->is_eigen (ratio)) {
				particle->set_E (E);
				MPI_Send (&E, 1, MPI_LONG_DOUBLE, 1, 1, MPI_COMM_WORLD);
			}
			
			else {
				E = 0.0;
				MPI_Send (&E , 1, MPI_LONG_DOUBLE, 1, 1, MPI_COMM_WORLD);
				break;
			}
// 			std::cerr << rank_ << std::endl;
// 			--i;
		}
		
	}

	else 
		if (rank_ == 1) {
			while (true) {
				particle->start_back (1e-10, 1e-10);
				ratio = particle->ratio();
// 				std::cerr << ratio << std::endl;
				MPI_Send (&ratio, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
				MPI_Recv (&E, 1, MPI_LONG_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				
				if (E == 0.0) break;
				else particle->set_E (E);
// 				std::cerr << rank_ << std::endl;
// 				--i;
			}
		}
	
	else {
		MPI_Finalize();
		return 0;
	}
	
	MPI_Finalize();
	return 0;
}
