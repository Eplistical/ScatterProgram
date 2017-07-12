#ifndef _SCATTER_GRID_VAR_HPP
#define _SCATTER_GRID_VAR_HPP
// variables in grid
#include "types.hpp"
#include <vector>
#include "json_toolkit.hpp"

namespace scatter{
    namespace grid{
		extern std::vector<DOUBLE_T> rmin;
		extern std::vector<DOUBLE_T> rmax;
		extern std::vector<UINT_T> Nr;

		// fef related 
		extern DOUBLE_T bandwidth;
		extern std::vector<DOUBLE_T> derange;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
    };
};

#endif
