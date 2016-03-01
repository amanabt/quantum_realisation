#ifndef __FINITE_WELL__
#define __FINITE_WELL__


class finite_well
{
public:
	finite_well (long double a = 1.0,
				 long double E = 5e-8,
				 long double m = 1.0,
				 long double dx = 1e-5) :
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
	void set_Energy (long double E) { _E = E; }

private:
	auto second_derivative (long double V) const -> long double;

private:
	void slope (long double V);

private:
	void write (long double x, long double psi) const;

private:
	bool is_eigen (long double ratio) const 
		{ return (_psi / _slope) == ratio; }

private:
	long double _a;
	long double _E;
	long double _m;
	long double _dx;
	long double _psi;
	long double _slope;
};

#endif
