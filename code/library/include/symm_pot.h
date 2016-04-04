#ifndef __SYMM_POT__
#define __SYMM_POT__

#include <cmath>

// // namespace potential
// {

class spherical_sym {
// public:
// 	spherical_sym (double Q = 1.0,
// 				   double R = 1.0) :
// 				   _Q (Q),
// 				   _R (R)
// 	{}

public:
	double operator () (double r) const 
	{
		double _R = 0.5;
		if (r <= _R)
			return lR (r);
		
		else
			return mR (r);
	}
	
private:
	double lR (double r) const
	{
		double _Q = 1.0;
		double _R = 0.5;
		return -1.5 * _Q / _R + 0.5 * _Q / pow (_R, 3) * pow (r, 2);
	}
	
	double mR (double r) const 
	{
		double _Q = 1.0;
		return -_Q / r;
	}
	
// private:
// 	double _Q;
// 	double _R;
};
// }; //namespace potential
#endif
