#ifndef _SURFACES_STEP_HPP
#define _SURFACES_STEP_HPP
// step surface
#include "scatter_basic.hpp"

#include <vector>
#include <cmath>
#include <sstream>
#include "basesurf.hpp"

namespace surfaces_collection{
    class step : public basesurf{
        private:
        DOUBLE_T A;
        DOUBLE_T B;
        DOUBLE_T C;

        public:
        explicit step(const std::vector<DOUBLE_T>& paralist){
            Npara = 3;
            para = std::vector<DOUBLE_T>(paralist.begin(), paralist.begin() + Npara);
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

        DOUBLE_T U(DOUBLE_T x) const{
            return (x <= C)?A:B;
        }

        DOUBLE_T dUdx(DOUBLE_T x) const{
            return 0.0;
        }
    };
};

#endif
