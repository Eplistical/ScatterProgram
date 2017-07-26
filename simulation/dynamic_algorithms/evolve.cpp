#include "scatter_basic.hpp"
#include "rem.hpp"
#include "vector.hpp"
#include "simulation.hpp"
#include "evolve.hpp"

using scatter::simulation::particle_t;
using scatter::simulation::mass;
using scatter::simulation::dt;

// API
VOID_T scatter::simulation::velocity_verlet(particle_t& ptcl, enumspace::dynamic_mode_enum mode, forcefunc_t forcefunc){
#if _DEBUG >= 4
	cout << "velocity_verlet ... ";
#endif 
    std::vector<DOUBLE_T>&& force = forcefunc(ptcl, mode);
    ptcl.p = ptcl.p + 0.5 * dt * force;
    ptcl.r = ptcl.r + dt * ptcl.p / mass;
    force = forcefunc(ptcl, mode);
    ptcl.p = ptcl.p + 0.5 * dt * force;
#if _DEBUG >= 4
	cout << "done. ";
#endif 
}
