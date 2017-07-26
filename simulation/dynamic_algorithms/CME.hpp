#ifndef _SCATTER_CME_HPP
#define _SCATTER_CME_HPP
// module for CME & bCME
#include "scatter_basic.hpp"
#include "vector.hpp"
#include "simulation.hpp"

namespace scatter
{
	namespace simulation
	{
		extern std::unordered_map< enumspace::dynamic_mode_enum, std::vector<scatter::simulation::hop_t> > hop_recorder;

		VOID_T CME(particle_t& ptcl, UINT_T trajID);
		VOID_T BCME(particle_t& ptcl, UINT_T trajID);
	};
};

#endif
