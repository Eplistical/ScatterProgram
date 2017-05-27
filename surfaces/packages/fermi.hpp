#ifndef _SURFACES_FERMI_HPP
#define _SURFACES_FERMI_HPP
// fermi surface

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces{
namespace packages{
    class fermi : public basesurf{
        private:
        double A;
        double B;
        double C;
        double D;
        
        public:
        explicit fermi(const std::vector<double>& paralist){
            Npara = 4;
            para = std::vector<double>(paralist.begin(), paralist.begin() + Npara);
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

        double U(double x) const{
            double tmp = exp(-C * (x - D));
            return B + isinf(tmp)?0.0:A / tmp;
        }

        double dUdx(double x) const{
            double tmp = exp(-C * (x - D));
            return isinf(tmp)?0.0:A * C * tmp / pow(1.0 + tmp, 2);
        }
    };
};
};

#endif
