#ifndef _SURFACES_FERMI_HPP
#define _SURFACES_FERMI_HPP
// fermi surface
#include "scatter_basic.hpp"

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection
{
    class fermi : public basesurf
	{
        private:
			DOUBLE_T A;
			DOUBLE_T B;
			DOUBLE_T C;
			DOUBLE_T D;
        
        public:
			explicit fermi(const std::vector<DOUBLE_T>& paralist)
			{
				Npara = 4;
				para = std::vector<DOUBLE_T>(paralist.begin(), paralist.begin() + Npara);
				A = para.at(0);
				B = para.at(1);
				C = para.at(2);
				D = para.at(3);

				mode << "fermi" << std::endl;
				expr << "U(x) = A / (1 + exp(-C * (x - D))) + B" << std::endl
					 << "A = " << A << std::endl
					 << "B = " << B << std::endl
					 << "C = " << C << std::endl
					 << "D = " << D << std::endl
					 ;
			}

			DOUBLE_T U(DOUBLE_T x) const noexcept
			{
				DOUBLE_T tmp = exp(-C * (x - D));
				return B + (isinf(tmp) ? 0.0 : A / (1.0 + tmp));
			}

			DOUBLE_T dUdx(DOUBLE_T x) const noexcept
			{
				DOUBLE_T tmp = exp(-C * (x - D));
				return isinf(tmp) ? 0.0 : A * C * tmp / pow(1.0 + tmp, 2);
			}
    };
};

#endif
