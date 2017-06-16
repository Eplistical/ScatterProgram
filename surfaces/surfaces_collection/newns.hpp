#ifndef _SURFACES_NEWNS_HPP
#define _SURFACES_NEWNS_HPP
// newns surface

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection{
    class newns : public basesurf{
        private:
        double A;
        double B;
        double C;
        double D;
        
        public:
        explicit newns(const std::vector<double>& paralist){
            Npara = 4;
            para = std::vector<double>(paralist.begin(), paralist.begin() + Npara);
            A = para.at(0);
            B = para.at(1);
            C = para.at(2);
            D = para.at(3);

            mode << "newns" << std::endl;
            expr << "U(x) = B + 1 / 4 / (x - D) + A / (x - D - C)^6" << std::endl
                 << "A = " << A << std::endl
                 << "B = " << B << std::endl
                 << "C = " << C << std::endl
                 << "D = " << D << std::endl
                 ;
        }

        double U(double x) const{
            x -= D;
            return B + 0.25 * pow(x, -1) + A * pow(x - C, -6);
        }

        double dUdx(double x) const{
            x -= D;
            return -0.25 * pow(x, -2) - 6 * A * pow(x - C, -7);
        }
    };
};

#endif
