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

enum TAG
{
	RATIO,
	ENERGY,
	PSI,
	__END__
};

void plot (void);

auto finite_well::second_derivative (double V) const -> double
{
	return s * (V - _E) * _psi;
}

// // // // // // // // // // // // // // // // // // // // // // // // 

void finite_well::slope (double V)
{
	_slope = _slope + this -> second_derivative (V) * _dx;
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
		slope (-10);
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
	double x = 1.2;
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
	
bool pipe (const std::string cmd)
{
	std::cerr << cmd << std::endl;
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
};// namespace utils

// // // // // // // // // // // // // // // // // // // // // // // // // // // // 

int main (int argc, char **argv) 
{
	int rank_;
	std::unique_ptr<finite_well> particle (new finite_well);
	double dE = -1e-6, E;
	double ratio;
	double psi = 0.0, factor = 1.0;

// 	auto i = 1e3;
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
			particle->start_front (0.0, 1e-3);
			MPI_Recv (&ratio, 1, MPI_LONG_DOUBLE, 1,
					  TAG::RATIO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
// 			std::cerr << ratio << std::endl;
			E = particle->E() + dE;
			if (!particle->is_eigen (ratio)) {
				particle->set_E (E);
				MPI_Send (&E, 1, MPI_LONG_DOUBLE, 1, TAG::ENERGY, MPI_COMM_WORLD);
			}
			
			else {
				E = 0.0;
				MPI_Send (&E , 1, MPI_LONG_DOUBLE, 1, TAG::ENERGY, MPI_COMM_WORLD);
				break;
			}
// 			std::cerr << rank_ << std::endl;
// 			--i;
		}
		
// 		std::cerr << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
		MPI_Recv (&psi, 1, MPI_LONG_DOUBLE, 1,
					  TAG::PSI, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		factor = particle -> multiplier (psi);
		std::string command[] = 
			{"cd ",
			 " && ",
			 " python multiplier.py "};
		command[2] += std::to_string (factor);
		char dir[100];
		getcwd(dir, 100);
		command [0] += dir;
		command [0] += " ";
// 		std::cerr << dir << std::endl;
		auto cmd = (command[0] + command [1] + command [2]);
// 		std::cerr << (command[0] + command [1] + command [2]) << std::endl;
		utils::pipe((command[0] + command [1] + command [2]));
	}

	else 
		if (rank_ == 1) {
			while (true) {
				particle->start_back (-1e-5, 1e-10);
				ratio = particle->ratio();
// 				std::cerr << ratio << std::endl;
				MPI_Send (&ratio, 1, MPI_LONG_DOUBLE, 0, TAG::RATIO, MPI_COMM_WORLD);
				MPI_Recv (&E, 1, MPI_LONG_DOUBLE, 0, TAG::ENERGY,
						  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				
				if (E == 0.0) {
					psi = particle -> psi();
					MPI_Send (&psi, 1, MPI_LONG_DOUBLE, 0, TAG::PSI, MPI_COMM_WORLD);
					break;
				}
				else particle->set_E (E);
// 				std::cerr << rank_ << std::endl;
// 				--i;
			}
		}
	else {
		MPI_Finalize();
		return 0;
	}
// 	std::cerr << "Exited : " << rank_ << std::endl;
	
	MPI_Finalize();
	return 0;
}
