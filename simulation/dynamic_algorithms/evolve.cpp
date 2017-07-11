#include "types.hpp"
#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "rem.hpp"
#include "vector.hpp"
#include "simulation.hpp"
#include "evolve.hpp"

using namespace scatter::simulation;
using scatter::particle_t;

// API
void scatter::simulation::velocity_verlet(particle_t& ptcl, enumspace::dynamics_mode_enum mode, forcefunc_t forcefunc){
#if _DEBUG >= 4
	cout << "velocity_verlet ... ";
#endif 
    std::vector<double>&& force = forcefunc(ptcl, mode);
    ptcl.p = ptcl.p + 0.5 * dt * force;
    ptcl.r = ptcl.r + dt * ptcl.p / mass;
    force = forcefunc(ptcl, mode);
    ptcl.p = ptcl.p + 0.5 * dt * force;
#if _DEBUG >= 4
	cout << "done. ";
#endif 
}
