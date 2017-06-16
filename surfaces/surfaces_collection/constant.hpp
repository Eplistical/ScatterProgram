#ifndef _SURFACES_CONSTANT_HPP
#define _SURFACES_CONSTANT_HPP
// constant surface

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection{
    class constant : public basesurf{
        private:
        double A;

        public:
        explicit constant(const std::vector<double>& paralist){
            Npara = 1;
            para = std::vector<double>(paralist.begin(), paralist.begin() + Npara);
            A = para.at(0);

            mode << "constant" << std::endl;
            expr << "U(x) = A" << std::endl
                 << "A = " << A << std::endl
                 ;
        }

        double U(double x) const{
            return A;
        }

        double dUdx(double x) const{
            return 0.0;
        }
    };
};

#endif
