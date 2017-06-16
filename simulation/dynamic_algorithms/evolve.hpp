#ifndef _SCATTER_EVOLVE_HPP
#define _SCATTER_EVOLVE_HPP
// module for evolve

#include "simulation.hpp"

namespace scatter{
	namespace simulation{
		using forcefunc_t = std::vector<double> (&)(const particle_t&, enumspace::dynamics_mode_enum);

		void velocity_verlet( particle_t& ptcl, enumspace::dynamics_mode_enum mode, forcefunc_t forcefunc );
		void runge_kutta_4th( particle_t& ptcl, enumspace::dynamics_mode_enum mode, forcefunc_t RK4_g );
	};
};

#endif
