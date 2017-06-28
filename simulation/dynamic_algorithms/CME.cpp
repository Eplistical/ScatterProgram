#include "ioer.hpp"
#include "randomer.hpp"
#include "vector.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "run.hpp"
#include "evolve.hpp"
#include "dynamics_mode.hpp"
#include "fermi.hpp"

using namespace scatter;
using namespace scatter::simulation;
using scatter::surfaces_obj;
using scatter::grid_obj;

using rem::hbar;
using rem::kT;
using rem::Gamma0;

using simulation::dt;
using surfaces::surfnum;

// hopper counter for CME & BCME
std::vector<hop_t> scatter::simulation::CME_hop_recorder;
std::vector<hop_t> scatter::simulation::BCME_hop_recorder;

static size_t _CME_hopper(const particle_t& ptcl, size_t trajID, enumspace::dynamics_mode_enum mode){
	// deal with hopping & ptclomentum adjustment
	const double gamma = surfaces_obj.fGamma(ptcl.r, Gamma0);
	const size_t hop_from = ptcl.surf;
	const double from_energy = surfaces_obj.fU(hop_from, ptcl.r);
	static std::vector<double> h(surfnum);
	static std::vector<double> Phop(surfnum);

	Phop[hop_from] = hbar / dt / gamma;
	h[hop_from] = 0.0;
	for(size_t i = 0; i < surfnum; ++i){
		if(i == hop_from) {
			continue;
		}
		else if(i > hop_from) {
			// |fewer el> -> |more el>
			h[i] = surfaces_obj.fU(i, ptcl.r) - from_energy;
			Phop[i] = Fermi(h[i] / kT);
		}
		else {
			// |more el> -> |less el>
			h[i] = from_energy - surfaces_obj.fU(i, ptcl.r);
			Phop[i] = 1 - Fermi(h[i] / kT);
		}
		Phop[hop_from] -= Phop[i];
	}
	// get hop_to according to the probabilities
	const size_t hop_to = randomer::discrete(Phop);
	if (hop_from != hop_to) {
		double to_energy = surfaces_obj.fU(hop_to, ptcl.r);
		// hopping happens
		if(mode == enumspace::dynamics_mode_enum::CME){
			CME_hop_recorder.push_back(hop_t(trajID, hop_from, hop_to, to_energy - from_energy, gamma, ptcl.r, ptcl.p));
		}
		else if(mode == enumspace::dynamics_mode_enum::BCME){
			BCME_hop_recorder.push_back(hop_t(trajID, hop_from, hop_to, to_energy - from_energy, gamma, ptcl.r, ptcl.p));
		}
	}
	return hop_to;
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
