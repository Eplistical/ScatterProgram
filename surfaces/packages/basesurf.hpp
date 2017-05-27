#ifndef _SURFACES_BASE_HPP
#define _SURFACES_BASE_HPP
// newns surface

#include <vector>
#include <cstring>
#include <sstream>

namespace surfaces{
namespace packages{
    class basesurf{
        public:
        basesurf(){};
        ~basesurf(){};

        virtual double U(double x) const = 0;
        virtual double dUdx(double x) const = 0;

        // get para number
        size_t N(void) const
        { return Npara; }

        // get expression string
        std::string get_expr(void) const
        { return expr.str(); }

        // get para vector
        std::vector<double> get_para(void) const
        { return para; }

        // get mode name
        std::string get_mode(void) const
        { return mode.str(); }


        protected:
        std::stringstream mode;
        std::stringstream expr;
        std::vector<double> para;
        size_t Npara;
    };
};
};

#endif
