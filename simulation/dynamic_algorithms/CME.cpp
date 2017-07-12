#include "types.hpp"
#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "io.hpp"
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
using rem::dim;

using simulation::dt;
using surfaces::surfnum;

// hopper counter for CME & BCME
std::vector<hop_t> scatter::simulation::CME_hop_recorder;
std::vector<hop_t> scatter::simulation::BCME_hop_recorder;

static UINT_T _CME_hopper(const particle_t& ptcl, UINT_T trajID, enumspace::dynamics_mode_enum mode){
	// deal with hopping & ptclomentum adjustment
#if _DEBUG >= 4
	cout << "debug: Now hopping ... ";
#endif 
	const DOUBLE_T gamma = surfaces_obj.fGamma(ptcl.r, Gamma0);
	const UINT_T hop_from = ptcl.surf;
	const DOUBLE_T from_energy = surfaces_obj.fU(hop_from, ptcl.r);
	static std::vector<DOUBLE_T> h(surfnum);
	static std::vector<DOUBLE_T> Phop(surfnum);

	Phop[hop_from] = hbar / dt / gamma;
	h[hop_from] = 0.0;

	for(UINT_T i = 0; i < surfnum; ++i){
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
	const UINT_T hop_to = randomer::discrete(Phop);
	if (hop_from != hop_to) {
#if _DEBUG >= 4
	cout << hop_from << " -> " << hop_to << "! ";
#endif 
		DOUBLE_T to_energy = surfaces_obj.fU(hop_to, ptcl.r);
		// hopping happens
		if(mode == enumspace::dynamics_mode_enum::CME){
			CME_hop_recorder.push_back(hop_t(trajID, hop_from, hop_to, to_energy - from_energy, gamma, ptcl.r, ptcl.p));
		}
		else if(mode == enumspace::dynamics_mode_enum::BCME){
			BCME_hop_recorder.push_back(hop_t(trajID, hop_from, hop_to, to_energy - from_energy, gamma, ptcl.r, ptcl.p));
		}
	}
#if _DEBUG >= 4
	cout << "done. ";
#endif 
	return hop_to;
}

static std::vector<DOUBLE_T> _CME_get_Force(const particle_t& ptcl, enumspace::dynamics_mode_enum mode) {
	// calc force on the particle
#if _DEBUG >= 4
	cout << "Getting force ... ";
#endif 
	std::vector<DOUBLE_T> force = surfaces_obj.fF(ptcl.surf, ptcl.r);
	if(mode == enumspace::dynamics_mode_enum::BCME) {
		force = force + grid_obj.get_fBCME(ptcl.r);
	}
	return force;
#if _DEBUG >= 4
	cout << "done. ";
#endif 
}

static void _CME(particle_t& ptcl, UINT_T trajID, enumspace::dynamics_mode_enum mode) {
#if _DEBUG >= 4
	cout << "debug: _CME: dynamic mode is " << enumspace::dynamics_mode_dict.right.at(mode) << ". ";
#endif

 	ptcl.hop( _CME_hopper(ptcl, trajID, mode) );
    velocity_verlet(ptcl, mode, _CME_get_Force);

#if _DEBUG >= 4
	cout << "\n";
#endif 
}

// API
void scatter::simulation::CME(particle_t& ptcl, UINT_T trajID) {
	 _CME(ptcl, trajID, enumspace::dynamics_mode_enum::CME);
}

void scatter::simulation::BCME(particle_t& ptcl, UINT_T trajID) {
	 _CME(ptcl, trajID, enumspace::dynamics_mode_enum::BCME);
}
