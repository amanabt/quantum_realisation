#include "../include/inf_well.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>

static constexpr auto h = 1;
static constexpr auto pi = 3.14159265359;
static constexpr auto s = 2 * 1.0 / pow (h / (2 * pi), 2.0);
void plot (void);
auto inf_well::second_derivative (long double V) const -> long double
{
	return s * (V - _E) * _psi;
}

void inf_well::slope (long double V)
{
	_slope = _slope + this -> second_derivative (V);
}

void inf_well::start (void)
{
	long double x = 0.0;
	std::fstream fs;
	std::cin >> _E >> _psi;
// 	while (true) {
			std::cerr << "Hello" << std::endl;
		while (x <= _a) {
			_psi = _psi + _slope * _dx;
			write (x, _psi);
			slope (x*x);
// 			_dx = _slope * _dx / 10;
			x += _dx;
			}
// 			if ((_psi < 1e-40) && (_psi > - 1e-40) != 0) {
// 				break;
// 			}
// 			else {
// 				_E += 1e-25;
// 				std::cerr << _E << std::endl;
// 				remove ("test.txt");
// 			};
// 	};
}

void inf_well::write (long double x, long double psi) const
{
	std::ofstream ofs;
	ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);
	ofs << x << "," << psi << std::endl;
// 	ofs.close();
}

void plot (void)
{
// 	static std::uint8_t flag = 0;
	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
	std::string commandsForGnuplot[] = {"set datafile separator \",\"", "y = 0", "plot y"};
	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[0].c_str());
	fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[1].c_str());
// 	else fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[2].c_str());
	std::this_thread::sleep_for (std::chrono::seconds(1));
// 	fclose(gnuplotPipe);
}

int main (void) 
{
	inf_well obj;
	std::cerr << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
	obj.start ();
	return 9;
}
