#ifndef _SURFACES_GAUSSIAN_HPP
#define _SURFACES_GAUSSIAN_HPP
// gaussian surface
#include "scatter_basic.hpp"

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection
{
    class gaussian : public basesurf
	{
        private:
			DOUBLE_T A;
			DOUBLE_T B;
			DOUBLE_T C;
			DOUBLE_T D;
        
        public:
        explicit gaussian(const std::vector<DOUBLE_T>& paralist)
		{
            Npara = 4;
            para = std::vector<DOUBLE_T>(paralist.begin(), paralist.begin() + Npara);
            A = para.at(0);
            B = para.at(1);
            C = para.at(2);
            D = para.at(3);

            mode << "gaussian" << std::endl;
            expr << "U(x) = A * exp(-C * (x - D)^2) + B" << std::endl
                 << "A = " << A << std::endl
                 << "B = " << B << std::endl
                 << "C = " << C << std::endl
                 << "D = " << D << std::endl
                 ;
        }

        DOUBLE_T U(DOUBLE_T x) const noexcept
		{
            return A * exp(-C * pow(x - D, 2)) + B;
        }

        DOUBLE_T dUdx(DOUBLE_T x) const noexcept
		{
            return -2.0 * A * C * x * exp(B * pow(x - D, 2));
        }
    };
};

#endif
