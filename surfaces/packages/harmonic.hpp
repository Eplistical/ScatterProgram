#ifndef _SURFACES_HARMONIC_HPP
#define _SURFACES_HARMONIC_HPP
// harmonic surface

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces{
namespace packages{

    class harmonic : public basesurf{
        private:
        double A;
        double B;
        double C;

        public:
        explicit harmonic(const std::vector<double>& paralist){
            Npara = 3;
            para = std::vector<double>(paralist.begin(), paralist.begin() + Npara);
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

        double U(double x) const{
            return 0.5 * A * pow(x - C, 2) + B;
        }

        double dUdx(double x) const{
            return A * (x - C);
        }
    };
};
};

#endif
