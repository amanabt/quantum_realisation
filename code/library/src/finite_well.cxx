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

static constexpr auto h = 6.6e-34;
static constexpr auto pi = 3.14159265359;
static constexpr auto s = 2 * 9.1e-31 / pow (h / (2 * pi), 2.0);

void plot (void);

bool finite_well::is_eigen (long double psi,
							long double slope) const
{
// 	std::cerr << std::fabs (psi - _psi) << ":" 
// 			  << std::fabs (slope - _slope) << ":"
// 			  << _E
// 			  << std::endl;
	if ((std::fabs (psi - _psi) <= 1e-7) && (std::fabs (slope - _slope) <= 1e-3)) {
		return true;
	}

	return false;
	
	
}

auto finite_well::second_derivative (long double V) const -> long double
{
	return s * (V - _E) * _psi;
}

// // // // // // // // // // // // // // // // // // // // // // // // 

void finite_well::slope (long double V)
{
	_slope = _slope + this -> second_derivative (V);
}

// // // // // // // // // // // // // // // // // // // // // // // // 

auto finite_well::start_front (long double slope_,
							   long double psi) -> long double
{
	remove("test_front.txt");
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

	return _E;
}

// // // // // // // // // // // // // // // // // // // // // // // // 

auto finite_well::start_back (long double slope_,
							  long double psi) -> long double
{
	remove ("test_back.txt");
	_slope = slope_;
	_psi = psi;
	std::ofstream ofs;
	ofs.open ("test_back.txt", std::ofstream::out | std::ofstream::app);
	long double x = 1.1e-16;
	if (_dx > 0.0) _dx = _dx * -1;
	while (x > _a) {
		_psi = _psi + _slope * _dx;
// 		write (x, _psi);
		{
			ofs << x << "," << _psi << std::endl;
			std::cerr << x << "," << _psi << std::endl;
		}
		slope (0.0);
		x += _dx;
	}

	return _E;
}

// // // // // // // // // // // // // // // // // // // // // // // // // 

void finite_well::write (long double x, long double psi) const
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
	long double dE = -1e-30, E;
	long double psi, slope;

	auto i = 100;
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
		while (i > 99){
			particle->start_front (1e-3, 0.0);
			MPI_Recv (&psi, 1, MPI_LONG_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv (&slope, 1, MPI_LONG_DOUBLE, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			E = particle->E() + dE;
			if (!particle->is_eigen (psi, slope)) {
				particle->set_E (E);
				MPI_Send (&E, 1, MPI_LONG_DOUBLE, 1, 2, MPI_COMM_WORLD);
			}
			
			else {
				E = 0.0;
				MPI_Send (&E , 1, MPI_LONG_DOUBLE, 1, 2, MPI_COMM_WORLD);
				break;
			}
			--i;
			std::cerr << i << std::endl;
		}
		
	}

	else 
		if (rank_ == 1) {
			while (i > 99) {
				particle->start_back (1e-300, 1e-300);
				psi = particle -> psi();
				slope = particle -> slope ();
				MPI_Send (&psi, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
				MPI_Send (&slope, 1, MPI_LONG_DOUBLE, 0, 1, MPI_COMM_WORLD);
				MPI_Recv (&E, 1, MPI_LONG_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				
				if (E == 0.0) break;
				else particle->set_E (E);
				--i;
			}
		}
	
	else {
		MPI_Finalize();
		return 0;
	}
	MPI_Finalize();
	return 0;
}
