#ifndef _SURFACES_HARMONIC_HPP
#define _SURFACES_HARMONIC_HPP
// harmonic surface
#include "scatter_basic.hpp"

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection{
    class harmonic : public basesurf{
        private:
        DOUBLE_T A;
        DOUBLE_T B;
        DOUBLE_T C;

        public:
        explicit harmonic(const std::vector<DOUBLE_T>& paralist){
            Npara = 3;
            para = std::vector<DOUBLE_T>(paralist.begin(), paralist.begin() + Npara);
            A = para.at(0);
            B = para.at(1);
            C = para.at(2);

            mode << "harmonic" << std::endl;
            expr << "U(x) = 0.5 * A * (x - C)^2 + B " << std::endl
                 << "A = " << A << std::endl
                 << "B = " << B << std::endl
                 << "C = " << C << std::endl
                 ;
        }

        DOUBLE_T U(DOUBLE_T x) const{
            return 0.5 * A * pow(x - C, 2) + B;
        }

        DOUBLE_T dUdx(DOUBLE_T x) const{
            return A * (x - C);
        }
    };
};

#endif
