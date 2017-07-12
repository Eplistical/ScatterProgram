#ifndef _SURFACES_BASE_HPP
#define _SURFACES_BASE_HPP
// newns surface
#include "types.hpp"

#include <vector>
#include <cstring>
#include <sstream>

namespace surfaces_collection{
    class basesurf{
        public:
			basesurf(){};
			~basesurf(){};

			virtual DOUBLE_T U(DOUBLE_T x) const = 0;
			virtual DOUBLE_T dUdx(DOUBLE_T x) const = 0;

			// getters
			UINT_T get_Npara(void) const { return Npara; }
			STRING_T get_expr(void) const { return expr.str(); }
			std::vector<DOUBLE_T> get_para(void) const { return para; }
			STRING_T get_mode(void) const { return mode.str(); }

        protected:
			std::stringstream mode;		// STRING_T for surf mode
			std::stringstream expr;		// STRING_T for expression
			std::vector<DOUBLE_T> para;	// vector for parameters
			UINT_T Npara;				// number of parameters required
    };
};

#endif
