#ifndef _SCATTER_GRID_VAR_HPP
#define _SCATTER_GRID_VAR_HPP
// variables in grid
#include <vector>
#include "json_toolkit.hpp"

namespace scatter{
    namespace grid{
		extern const std::vector<double>& rmin;
		extern const std::vector<double>& rmax;
		extern const std::vector<size_t>& Nr;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
    };
};

#endif
