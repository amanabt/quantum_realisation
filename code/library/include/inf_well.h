#ifndef __INF_WELL__
#define __INF_WELL__


class inf_well
{
public:
	inf_well (long double a = 1.0,
			  long double E = 0.00000023,
			  long double slope = 10.0,
			  long double dx = 1e-4,
			  long double m = 1.0) :
			_a (a),
			_E (E),
			_slope (slope),
			_dx (dx),
			_m (m)
			{}

public:
	void start (void);

private:
	auto second_derivative (long double V) const -> long double;

private:
	void slope (long double V);

private:
	void write (long double x, long double psi) const;

private:
	bool is_eigen (long double x) const { return (_psi == 0) && (x == _a);}
	
private:
	long double _a;
	long double _E;
	long double _slope;
	long double _dx;
	long double _m;
	long double _psi;
};

#endif















// 	long double set_width (long double a);
// 
// public:
// 	long double set_dx (long double dx);
// 
// public:
// 	long double set_E (long double E);
// 
// public:
// 	long double set_init_slope (long double init_slope);
