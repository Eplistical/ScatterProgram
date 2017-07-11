#ifndef _SCATTER_EVOLVE_HPP
#define _SCATTER_EVOLVE_HPP
// module for evolve
#include "types.hpp"

#include "simulation.hpp"

namespace scatter{
	namespace simulation{
		using forcefunc_t = std::vector<double> (&)(const particle_t&, enumspace::dynamics_mode_enum);

		void velocity_verlet( particle_t& ptcl, enumspace::dynamics_mode_enum mode, forcefunc_t forcefunc );
	};
};

#endif
