#include "ioer.hpp"
#include "vector.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "evolve.hpp"
#include "dynamics_mode.hpp"

using namespace scatter;
using namespace scatter::simulation;

// hopper counter for CME & BCME
std::vector<hop_t> scatter::simulation::CME_hop_recorder;
std::vector<hop_t> scatter::simulation::BCME_hop_recorder;

static int _CME_hopper(const particle_t& ptcl, size_t trajID, enumspace::dynamics_mode_enum mode){
	// deal with hopping & ptclomentum adjustment
	return ptcl.surf;
}

static std::vector<double> _CME_get_Force(const particle_t& ptcl, enumspace::dynamics_mode_enum mode) {
	// calc force for given particle
	std::vector<double> force(rem::dim);
	return force;
}

static void _CME(particle_t& ptcl, size_t trajID, enumspace::dynamics_mode_enum mode) {
 	ptcl.hop( _CME_hopper(ptcl, trajID, mode) );
    velocity_verlet(ptcl, mode, _CME_get_Force);
}

// API
void scatter::simulation::CME(particle_t& ptcl, size_t trajID) {
	 _CME(ptcl, trajID, enumspace::dynamics_mode_enum::CME);
}

void scatter::simulation::BCME(particle_t& ptcl, size_t trajID) {
	 _CME(ptcl, trajID, enumspace::dynamics_mode_enum::BCME);
}

