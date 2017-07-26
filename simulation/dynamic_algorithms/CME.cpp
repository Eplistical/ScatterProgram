#include "scatter_basic.hpp"
#include "io.hpp"
#include "randomer.hpp"
#include "vector.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "run.hpp"
#include "evolve.hpp"
#include "dynamic_mode.hpp"
#include "fermi.hpp"
using scatter::io::log_handler;

using namespace scatter;
using namespace scatter::simulation;
using scatter::surfaces_obj;
using scatter::grid_obj;

using rem::hbar;
using rem::kT;
using rem::Gamma0;
using rem::dim;

using simulation::dt;
using simulation::particle_t;
using surfaces::surfnum;

// hopper counter for CME & BCME
std::unordered_map< enumspace::dynamic_mode_enum, std::vector<hop_t> > 
scatter::simulation::hop_recorder 
{
	{ enumspace::dynamic_mode_enum::CME, std::vector<hop_t>() },
	{ enumspace::dynamic_mode_enum::BCME, std::vector<hop_t>() },
};

static UINT_T _CME_hopper(const particle_t& ptcl, UINT_T trajID, enumspace::dynamic_mode_enum mode)
{
	// deal with hopping & ptclomentum adjustment
#if _DEBUG >= 4
	log_handler.info("debug: Now hopping ... ");
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

	// record when hop happens
	if (hop_from != hop_to) {

#if _DEBUG >= 4
	log_handler.info(hop_from, " -> ", hop_to, "! ");
#endif 

		DOUBLE_T to_energy = surfaces_obj.fU(hop_to, ptcl.r);
		hop_recorder.at(mode).emplace_back(trajID, hop_from, hop_to, to_energy - from_energy, gamma, ptcl.r, ptcl.p);
	}

#if _DEBUG >= 4
	log_handler.info("done. ");
#endif 

	return hop_to;
}

static std::vector<DOUBLE_T> _CME_get_Force(const particle_t& ptcl, enumspace::dynamic_mode_enum mode) {
	// calc force on the particle
#if _DEBUG >= 4
	log_handler.info_nonewline("Getting force ... ");
#endif 
	std::vector<DOUBLE_T> force = surfaces_obj.fF(ptcl.surf, ptcl.r);
	if(mode == enumspace::dynamic_mode_enum::BCME) {
		force = force + grid_obj.get_fBCME(ptcl.r);
	}
	return force;
#if _DEBUG >= 4
	log_handler.info("done. ");
#endif 
}

static VOID_T _CME(particle_t& ptcl, UINT_T trajID, enumspace::dynamic_mode_enum mode) {
#if _DEBUG >= 4
	log_handler.info_nonewline("debug: _CME: dynamic mode is ", enumspace::dynamic_mode_dict.right.at(mode), ". ");
#endif

 	ptcl.hop( _CME_hopper(ptcl, trajID, mode) );
    velocity_verlet(ptcl, mode, _CME_get_Force);

#if _DEBUG >= 4
	log_handler.info("");
#endif 
}

// API
VOID_T scatter::simulation::CME(particle_t& ptcl, UINT_T trajID) {
	 _CME(ptcl, trajID, enumspace::dynamic_mode_enum::CME);
}

VOID_T scatter::simulation::BCME(particle_t& ptcl, UINT_T trajID) {
	 _CME(ptcl, trajID, enumspace::dynamic_mode_enum::BCME);
}
