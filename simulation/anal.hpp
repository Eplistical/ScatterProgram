#ifndef _SCATTER_SIMULATION_ANAL_HPP
#define _SCATTER_SIMULATION_ANAL_HPP
// module for data analysis
#include "types.hpp"

#include <vector>
#include "particle_t.hpp"
#include "analmode.hpp"

namespace scatter{
	namespace simulation{
		std::vector<DOUBLE_T> anal(const particle_t& ptcl, enumspace::analmode_enum mode);
	};
};

#endif
