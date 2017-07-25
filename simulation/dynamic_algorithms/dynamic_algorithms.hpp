#ifndef _SCATTER_DYNAMIC_ALGORITHMS_HPP
#define _SCATTER_DYNAMIC_ALGORITHMS_HPP
// module for dynamic algorithms
#include "scatter_basic.hpp"

#include <unordered_map>
#include "dynamic_mode.hpp"
#include "evolve.hpp"
#include "CME.hpp"
#include "EF.hpp"

namespace scatter
{
	namespace simulation
	{
		using DynamicFunctionPtrType = VOID_T(*)(particle_t&, UINT_T);

		extern std::unordered_map
				<enumspace::dynamic_mode_enum, DynamicFunctionPtrType> dynamic_algorithms;
	}
}

#endif
