#ifndef _SURFACES_STEP_HPP
#define _SURFACES_STEP_HPP
// step surface
#include "types.hpp"

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection{
    class step : public basesurf{
        private:
        double A;
        double B;
        double C;

        public:
        explicit step(const std::vector<double>& paralist){
            Npara = 3;
            para = std::vector<double>(paralist.begin(), paralist.begin() + Npara);
            A = para.at(0);
            B = para.at(1);
            C = para.at(2);

            mode << "step" << std::endl;
            expr << "U(x) = A (for x <= C)" << std::endl
                 << "U(x) = B (for x > C)" << std::endl
                 << "A = " << A << std::endl
                 << "B = " << B << std::endl
                 << "C = " << C << std::endl
                 ;
        }

        double U(double x) const{
            return (x <= C)?A:B;
        }

        double dUdx(double x) const{
            return 0.0;
        }
    };
};

#endif
