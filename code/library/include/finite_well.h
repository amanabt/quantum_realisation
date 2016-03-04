#ifndef __FINITE_WELL__
#define __FINITE_WELL__

#include <iostream>

class finite_well
{
public:
	finite_well (long double a = 1.0e-16,
				 long double E = -1e-17,
				 long double m = 9.1e-31,
				 long double dx = 1e-20) :
				 _a (a),
				 _E (E),
				 _m (m),
				 _dx (dx)
			{}

public:
	auto start_front (long double slope_, long double psi) -> long double;

public:
	auto start_back  (long double slope_, long double psi) -> long double;
	
public:
	void set_E (long double E) { _E = E; }
	
public:
	auto E (void) const -> long double { return _E; }
	
public:
	const auto ratio (void) const -> long double
	{ return _psi / _slope; }

public:
	long double psi (void) const { return this -> _psi; }
	
public:
	long double slope (void) const { return this -> _slope; }

public:
	bool is_eigen (long double psi, long double slope) const;

private:
	auto second_derivative (long double V) const -> long double;

private:
	void slope (long double V);

private:
	void write (long double x, long double psi) const;

private:
	long double _a;
	long double _E;
	long double _m;
	long double _dx;
	long double _psi;
	long double _slope;
};

#endif
