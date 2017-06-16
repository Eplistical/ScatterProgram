#ifndef _SCATTER_SURFACES_VAR_HPP
#define _SCATTER_SURFACES_VAR_HPP
// module for variables in surfaces
#include <cstdio>
#include <vector>
#include "json_toolkit.hpp"
#include "surfaces_collection.hpp"

namespace scatter{
	namespace surfaces{
		using enumspace::surfmode_enum;
		extern const size_t& surfnum;
		extern const double& cutoff_gamma;
		extern const std::vector<std::vector<surfmode_enum>>& surfmode;
		extern const std::vector<std::vector<double>>& surfpara;
		extern const std::vector<std::vector<surfmode_enum>>& gammamode;
		extern const std::vector<std::vector<double>>& gammapara;

		// fef related 
		extern const double& bandwidth;
		extern const std::vector<double>& derange;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
	};
};

#endif
