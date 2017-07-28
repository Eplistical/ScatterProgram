#ifndef _SCATTER_SURFACES_VAR_HPP
#define _SCATTER_SURFACES_VAR_HPP
// module for variables in surfaces
#include "scatter_basic.hpp"
#include <cstdio>
#include "vector.hpp"
#include "json_toolkit.hpp"
#include "surfaces_collection.hpp"

namespace scatter 
{
	namespace surfaces 
	{
		using enumspace::surfmode_enum;
		extern UINT_T surfnum;
		extern DOUBLE_T cutoff_gamma;
		extern std::vector<surfmode_enum> surfmode;
		extern std::vector<std::vector<DOUBLE_T> > surfpara;
		extern std::vector<surfmode_enum> gammamode;
		extern std::vector<std::vector<DOUBLE_T> > gammapara;

		VOID_T load_var(const rapidjson::Document& doc);
		VOID_T print_var(VOID_T);
		STRING_T get_surface_expr(enumspace::surfmode_enum mode, const std::vector<DOUBLE_T>& para);
	};
};

#endif
