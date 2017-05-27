#ifndef _SURFACES_MORSE_HPP
#define _SURFACES_MORSE_HPP

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces{
namespace packages{
    class morse : public basesurf{
        private:
        double A;
        double B;
        double C;
        double D;

        public:
        explicit morse(const std::vector<double>& paralist){
            Npara = 4;
            para = std::vector<double>(paralist.begin(), paralist.begin() + Npara);
            A = para.at(0);
            B = para.at(1);
            C = para.at(2);
            D = para.at(3);

            mode << "morse" << std::endl;
            expr << "U(x) = A * (1 - exp(C * (x - D)))^2 + B" << std::endl
                 << "A = " << A << std::endl
                 << "B = " << B << std::endl
                 << "C = " << C << std::endl
                 << "D = " << D << std::endl
                 ;
        }

        double U(double x) const{
            double tmp = exp(C * (x - D));
            return A * pow(1 - tmp, 2) + B;
        }

        double dUdx(double x) const{
            double tmp = exp(C * (x - D));
            return -2 * A * C * tmp * (1 - tmp); 
        }
    };
};
};

#endif
