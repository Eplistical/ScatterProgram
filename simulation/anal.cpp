#include "types.hpp"
#include "vector.hpp"
#include "io.hpp"
#include "rem.hpp"
#include "dynamic_algorithms.hpp"
#include "simulation_var.hpp"
#include "analmode.hpp"
#include "anal.hpp"
using scatter::io::out_handler;

using namespace scatter;
using namespace scatter::simulation;

static std::vector<DOUBLE_T> _rp(const std::vector<particle_t>& swarm) {
	// <r> and <p>
    /*
	std::vector<DOUBLE_T> rp;
	rp.assign(rem::dim * 2, 0);
	for(UINT_T traj = 0; traj < simulation::Ntraj; ++traj){
		for(UINT_T d = 0; d < rem::dim; ++d){
			rp[d] += swarm[traj].r[d];
			rp[d + rem::dim] += swarm[traj].p[d];
		}
	}
	return rp / simulation::Ntraj;
    */
    return std::vector<DOUBLE_T>(rem::dim * 2, 0.0);
}

static DOUBLE_T _surf(const std::vector<particle_t>& swarm) {
	// <n1>
    /*
	UINT_T surf = 0;
	for(UINT_T traj = 0; traj < simulation::Ntraj; traj++){
		surf += swarm[traj].surf;
	}
	return 1.0 * surf / simulation::Ntraj;
    */
    return 0.0;
}

static std::vector<DOUBLE_T> _Ep(const std::vector<particle_t>& swarm) {
	// potential energy
    /*
	std::vector<DOUBLE_T> Ep(rem::dim, 0);
	DOUBLE_T x, x0;
	for(INT_T traj = 0; traj < simulation::Ntraj; traj++){
		if(swarm[traj].surf == 0){
			Ep = Ep + vU(0, swarm[traj].r);
		}
		else if(swarm[traj].surf == 1){
			Ep = Ep + vU(1, swarm[traj].r);
		}
	}
	return Ep / simulation::Ntraj;
    */
    return std::vector<DOUBLE_T>(rem::dim, 0.0);
}

static std::vector<DOUBLE_T> _Ek(const std::vector<particle_t>& swarm) {
	// kinetic energy
    /*
	std::vector<DOUBLE_T> Ek(rem::dim, 0);
	for(UINT_T traj = 0; traj < simulation::Ntraj; ++traj){
		Ek = Ek + 0.5 * pow(swarm[traj].p, 2) / mass;
	}
	return Ek / simulation::Ntraj;
    */
    return std::vector<DOUBLE_T>(rem::dim, 0.0);
}

static VOID_T _default_anal(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode) {
    /*
	const std::vector<DOUBLE_T> rp = _rp(swarm);
	const std::vector<DOUBLE_T> Ek = _Ek(swarm);
	const DOUBLE_T surf = _surf(swarm);
	const DOUBLE_T z_boundary = -20;
	// output
	out_handler.tabout_nonewline(rp, 0, 2);
	out_handler.tabout_nonewline(Ek, surf);
	// number of particle goes out 
	UINT_T Nout = 0;
	for(UINT_T traj = 0; traj < simulation::Ntraj; ++traj){
		if(swarm[traj].r[1] < z_boundary and swarm[traj].p[1] < 0){
			++Nout;
		}
	}
	out_handler.tabout_nonewline(Nout);
    */
}

static VOID_T _final_dist_anal(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode) {
    /*
	DOUBLE_T Omega, Ekx, Epx;
	INT_T n_vib;
	// header
	out_handler.tabout(0,1,2,3,4,5,6,7,8);
	out_handler.tabout("traj", "n_el", "n_vib", "Ek_vib", "Ep_vib", "x", "px", "z", "pz");
	out_handler.drawline('#');
	// loop trajectories
	for(UINT_T traj = 0;traj < simulation::Ntraj; traj++){
		Omega = omega.at(swarm[traj].surf);
		Ekx = pow(swarm[traj].p[0], 2) / 2 / mass[0];
		Epx = surf.fU(swarm[traj].surf, 0, r[0];
		// round nvib
		n_vib = round((Ekx + Epx) / Omega - 0.5);

		out_handler.tabout(	traj, swarm[traj].surf, n_vib, Ekx, Epx,
						swarm[traj].r[0],
						swarm[traj].p[0],
						swarm[traj].r[1],
						swarm[traj].p[1]);
		out_handler.newline();
	}
	out_handler.drawline('#');
    */
}

static VOID_T _hop_anal_para(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode){
	/* analyze hoppings in CME/BCME
	 */
    /*
	std::vector<hop_t> hop_recorder;
	if(mode == enumspace::analmode_enum::HOP_ANAL_CME){
		hop_recorder = CME_hop_recorder;
		out_handler.info("hop_anal_CME:");
	}
	else if(mode == enumspace::analmode_enum::HOP_ANAL_BCME){
		hop_recorder = BCME_hop_recorder;
		out_handler.info("hop_anal_BCME:");
	}
	out_handler.tabout(0,1,2,3,4,5,6,7,8);
	out_handler.tabout("trajID", "from", "to", "energy_gap", "gamma", "x", "z", "px", "pz");
	out_handler.drawline('#');
	for(UINT_T i = 0, Nhop = hop_recorder.size(); i < Nhop; ++i){
		out_handler.tabout(	hop_recorder[i].id, 
						hop_recorder[i].from,
						hop_recorder[i].to,
						hop_recorder[i].energy_gap,
						hop_recorder[i].gamma,
						hop_recorder[i].r[0],
						hop_recorder[i].r[1],
						hop_recorder[i].p[0],
						hop_recorder[i].p[1]);
		out_handler.newline();
	}
	out_handler.drawline('#');
    */
}

// API
std::vector<DOUBLE_T> anal(const particle_t& ptcl, enumspace::analmode_enum mode){
	using namespace enumspace;
	switch(mode){
		/*
		case analmode_enum::SIMU_ANAL_CME :
		case analmode_enum::SIMU_ANAL_BCME :
		case analmode_enum::SIMU_ANAL_EF :
			_default_anal(swarm, mode);
			break;
		case analmode_enum::FINAL_ANAL_CME :
		case analmode_enum::FINAL_ANAL_BCME :
		case analmode_enum::FINAL_ANAL_EF :
			_final_dist_anal(swarm, mode);
			break;
		case analmode_enum::HOP_ANAL_CME:
		case analmode_enum::HOP_ANAL_BCME:
			_hop_anal_para(swarm, mode);
			*/
	}
}
