#ifndef _SURFACES_GAUSSIAN_HPP
#define _SURFACES_GAUSSIAN_HPP
// gaussian surface
#include "types.hpp"

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection{
    class gaussian : public basesurf{
        private:
        double A;
        double B;
        double C;
        double D;
        
        public:
        explicit gaussian(const std::vector<double>& paralist){
            Npara = 4;
            para = std::vector<double>(paralist.begin(), paralist.begin() + Npara);
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

        double U(double x) const{
            x -= D;
            return A * exp(-C * pow(x, 2)) + B;
        }

        double dUdx(double x) const{
            x -= D;
            return -2.0 * A * C * x * exp(-para.at(2) * pow(x, 2));
        }
    };
};

#endif
