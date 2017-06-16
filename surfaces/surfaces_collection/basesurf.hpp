#ifndef _SURFACES_BASE_HPP
#define _SURFACES_BASE_HPP
// newns surface

#include <vector>
#include <cstring>
#include <sstream>

namespace surfaces_collection{
    class basesurf{
        public:
			basesurf(){};
			~basesurf(){};

			virtual double U(double x) const = 0;
			virtual double dUdx(double x) const = 0;

			// getters
			size_t get_Npara(void) const { return Npara; }
			std::string get_expr(void) const { return expr.str(); }
			std::vector<double> get_para(void) const { return para; }
			std::string get_mode(void) const { return mode.str(); }

        protected:
			std::stringstream mode;		// string for surf mode
			std::stringstream expr;		// string for expression
			std::vector<double> para;	// vector for parameters
			size_t Npara;				// number of parameters required
    };
};

#endif
