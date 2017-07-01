#ifndef _SCATTER_GRID_VAR_HPP
#define _SCATTER_GRID_VAR_HPP
// variables in grid
#include <vector>
#include "json_toolkit.hpp"

namespace scatter{
    namespace grid{
		extern std::vector<double> rmin;
		extern std::vector<double> rmax;
		extern std::vector<size_t> Nr;

		// fef related 
		extern double bandwidth;
		extern std::vector<double> derange;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
    };
};

#endif
