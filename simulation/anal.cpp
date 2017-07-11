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

static std::vector<double> _rp(const std::vector<particle_t>& swarm) {
	// <r> and <p>
    /*
	std::vector<double> rp;
	rp.assign(rem::dim * 2, 0);
	for(size_t traj = 0; traj < simulation::Ntraj; ++traj){
		for(size_t d = 0; d < rem::dim; ++d){
			rp[d] += swarm[traj].r[d];
			rp[d + rem::dim] += swarm[traj].p[d];
		}
	}
	return rp / simulation::Ntraj;
    */
    return std::vector<double>(rem::dim * 2, 0.0);
}

static double _surf(const std::vector<particle_t>& swarm) {
	// <n1>
    /*
	size_t surf = 0;
	for(size_t traj = 0; traj < simulation::Ntraj; traj++){
		surf += swarm[traj].surf;
	}
	return 1.0 * surf / simulation::Ntraj;
    */
    return 0.0;
}

static std::vector<double> _Ep(const std::vector<particle_t>& swarm) {
	// potential energy
    /*
	std::vector<double> Ep(rem::dim, 0);
	double x, x0;
	for(int traj = 0; traj < simulation::Ntraj; traj++){
		if(swarm[traj].surf == 0){
			Ep = Ep + vU(0, swarm[traj].r);
		}
		else if(swarm[traj].surf == 1){
			Ep = Ep + vU(1, swarm[traj].r);
		}
	}
	return Ep / simulation::Ntraj;
    */
    return std::vector<double>(rem::dim, 0.0);
}

static std::vector<double> _Ek(const std::vector<particle_t>& swarm) {
	// kinetic energy
    /*
	std::vector<double> Ek(rem::dim, 0);
	for(size_t traj = 0; traj < simulation::Ntraj; ++traj){
		Ek = Ek + 0.5 * pow(swarm[traj].p, 2) / mass;
	}
	return Ek / simulation::Ntraj;
    */
    return std::vector<double>(rem::dim, 0.0);
}

static void _default_anal(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode) {
    /*
	const std::vector<double> rp = _rp(swarm);
	const std::vector<double> Ek = _Ek(swarm);
	const double surf = _surf(swarm);
	const double z_boundary = -20;
	// output
	out_handler.tabout_nonewline(rp, 0, 2);
	out_handler.tabout_nonewline(Ek, surf);
	// number of particle goes out 
	size_t Nout = 0;
	for(size_t traj = 0; traj < simulation::Ntraj; ++traj){
		if(swarm[traj].r[1] < z_boundary and swarm[traj].p[1] < 0){
			++Nout;
		}
	}
	out_handler.tabout_nonewline(Nout);
    */
}

static void _final_dist_anal(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode) {
    /*
	double Omega, Ekx, Epx;
	int n_vib;
	// header
	out_handler.tabout(0,1,2,3,4,5,6,7,8);
	out_handler.tabout("traj", "n_el", "n_vib", "Ek_vib", "Ep_vib", "x", "px", "z", "pz");
	out_handler.drawline('#');
	// loop trajectories
	for(size_t traj = 0;traj < simulation::Ntraj; traj++){
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

static void _hop_anal_para(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode){
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
	for(size_t i = 0, Nhop = hop_recorder.size(); i < Nhop; ++i){
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
void anal(const std::vector<particle_t>& swarm, enumspace::analmode_enum mode){
	using namespace enumspace;
	switch(mode){
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
	}
}
