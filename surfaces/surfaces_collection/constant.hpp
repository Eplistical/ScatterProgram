#ifndef _SURFACES_CONSTANT_HPP
#define _SURFACES_CONSTANT_HPP
// constant surface
#include "scatter_basic.hpp"

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection 
{
    class constant : public basesurf {
        private:
	        DOUBLE_T A;

        public:
			explicit constant(const std::vector<DOUBLE_T>& paralist)
			{
				Npara = 1;
				para = std::vector<DOUBLE_T>(paralist.begin(), paralist.begin() + Npara);
				A = para.at(0);

				mode << "constant" << std::endl;
				expr << "U(x) = A" << std::endl
					 << "A = " << A << std::endl
					 ;
			}

			DOUBLE_T U(DOUBLE_T x) const noexcept
			{
				return A;
			}

			DOUBLE_T dUdx(DOUBLE_T x) const noexcept
			{
				return 0.0;
			}
    };
};

#endif
