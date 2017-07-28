#ifndef _SCATTER_EVOLVE_HPP
#define _SCATTER_EVOLVE_HPP
// module for evolve
#include "scatter_basic.hpp"
#include "simulation.hpp"

namespace scatter
{
	namespace simulation
	{
		using forcefunc_t = std::vector<DOUBLE_T> (&)(const particle_t&, enumspace::dynamic_mode_enum);

		VOID_T velocity_verlet( particle_t& ptcl, enumspace::dynamic_mode_enum mode, forcefunc_t forcefunc );
	};
};

#endif
