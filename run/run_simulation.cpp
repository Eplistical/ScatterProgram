#include "vector.hpp"
#include "rem.hpp"
#include "io.hpp"
#include "simulation.hpp"
#include "run.hpp"

#if defined(_OPENMP)
#include "omp.h"
#endif

using namespace scatter;
using namespace scatter::simulation;
using namespace scatter::rem;

static void assign_initstate(std::vector<particle_t>& swarm){
	for(size_t traj = 0; traj < Ntraj; ++traj){
		swarm.at(traj).ranforce = std::vector<double>(dim, 0.0);
		swarm.at(traj).surf = 0;
		swarm.at(traj).r = std::vector<double>(r0p0.begin() + traj * dim * 2,
												r0p0.begin() + traj * dim * 2 + dim);
		swarm.at(traj).p = std::vector<double>(r0p0.begin() + traj * dim * 2 + dim,
												r0p0.begin() + traj * dim * 2 + dim * 2);
	}
}

void scatter::run_simulation(){
	/*
	const size_t Nswarm = 3;
	std::vector<std::vector<particle_t>> swarms(Nswarm);
	std::vector<std::vector<particle_t>> records(Nswarm);
	// get init file
	if(prepinit){
		generate_initstate();
		io::saveinit();
	}
	else{
		io::loadinit();
	}
	// init swarms
	for(size_t i = 0; i < Nswarm; ++i){
		swarms.at(i) = std::vector<particle_t>(Ntraj);
		assign_initstate(swarms.at(i));
	}
	size_t traj, step, irecord;
#if defined(_OPENMP)
	omp_set_num_threads(threadNum);
#pragma omp parallel for if(threadNum > 1)	\
	default(shared)                     \
	private(traj, step, irecord)        \
	schedule(dynamic)
#endif
	for(traj = 0; traj < Ntraj; ++traj){
		step = 0;
		irecord = 0;
		while(step < Nstep){
			// store anastep data
			if(step % Anastep == 0){
				records[0][traj + irecord * Ntraj] = swarms[0][traj];
				records[1][traj + irecord * Ntraj] = swarms[1][traj];
				records[2][traj + irecord * Ntraj] = swarms[2][traj];
				++irecord;
			}
			// evolve
			CME(swarms[0][traj], traj);
			BCME(swarms[1][traj], traj);
			EF(swarms[2][traj], traj);
			++step;
		}
	}
	*/
}
