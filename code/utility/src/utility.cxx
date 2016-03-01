#include "../include/utility.h"

void swap (long long int& a,
		   long long int& b)
{
	auto temp = a;
	a = b;
	b = temp;
}
