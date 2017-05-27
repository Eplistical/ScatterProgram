#ifndef _FERMI_HPP
#define _FERMI_HPP
// fermi function

#include <cmath>
inline double Fermi(double x){
	/* f(x) = 1 / (1 + e^x) */
	x = exp(x);
	return isinf(x)?0.0:1.0 / (1.0 + x);
}

#endif
