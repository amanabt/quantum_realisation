#ifndef __SPHERICAL_SYMM__
#define __SPHERICAL_SYMM__

#include <iostream>
#include <functional>
#include "../include/symm_pot.h"

class finite_well
{
public:
	finite_well (double R  = 0.5,
				 double E  = -5.0,
				 double m  = 1.0,
				 double dr = 1e-4,
				 double Q  = 1.0) :
				 _R  (R),
				 _E  (E),
				 _m  (m),
				 _dr (dr),
				 _V  (spherical_sym())
			{}

public:
	auto start_front (double slope_, double psi) -> double;

public:
	auto start_back  (double slope_, double psi) -> double;
	
public:
	void set_E (double E) { _E = E; }
	
public:
	auto E (void) const -> double { return _E; }
	
public:
	const auto ratio (void) const -> double
	{ return _psi / _slope; }

public:
	bool is_eigen (double ratio) const 
		{ std::cerr << ratio << ":"
					<< (_psi / _slope) << ":"
					<< (_psi / _slope) - ratio
					<< std::endl;
			
		  return ((_psi / _slope) - ratio) < 1e-3 && 
		  ((_psi / _slope) - ratio) > -1e-3;
		}

public:
	double multiplier (double psi) const 
	{ return psi / _psi; }
	
public:
	double psi (void) const
	{ return _psi; }

private:
	auto second_derivative (double V) const -> double;

private:
	void slope (double V);

private:
	void write (double x, double psi) const;

private:
	double                         _R;
	double                         _E;
	double                         _m;
	double                         _dr;
	double                         _psi;
	double                         _slope;
	std::function<double (double)> _V;
};

#endif
