#ifndef _SCATTER_EVOLVE_HPP
#define _SCATTER_EVOLVE_HPP
// module for evolve

#include "particle_t.hpp"

namespace scatter{
	namespace simulation{
		using forcefunc_t = std::vector<double> (&)(const particle_t&, int);

		void velocity_verlet( particle_t& ptcl, int mode, forcefunc_t forcefunc );
		void runge_kutta_4th( particle_t& ptcl, int mode, forcefunc_t RK4_g );
	};
};

#endif
