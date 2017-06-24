#ifndef _SCATTER_SIMULATION_ANAL_HPP
#define _SCATTER_SIMULATION_ANAL_HPP
// module for data analysis

#include <vector>
#include "particle_t.hpp"
#include "analmode.hpp"

namespace scatter{
	namespace simulation{
		void anal(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode);
	};
};

#endif
