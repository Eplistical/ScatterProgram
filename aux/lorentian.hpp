#ifndef _LORENTIAN_HPP
#define _LORENTIAN_HPP
// lorentian function
#include <cmath>
inline double Lorentian(double x, double gamma){
	/* L(x) = 1/pi * gamma / (x^2 + gamma^2) */
	return  gamma / (pow(x, 2) + pow(gamma, 2)) / M_PI;
}

#endif
