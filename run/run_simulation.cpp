#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "vector.hpp"
#include "rem.hpp"
#include "io.hpp"
#include "simulation.hpp"
#include "surfaces.hpp"
#include "grid.hpp"
#include "run.hpp"

#if defined(_OPENMP)
#include "omp.h"
#endif

using namespace scatter;
using namespace scatter::grid;
using namespace scatter::surfaces;
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
#if _DEBUG >= 1
	cout << "debug: run_simulation: begin" << "\n";
#endif
	grid_obj = grid_t(rmin, rmax, Nr);
	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);
	const size_t Nswarm = 3, Nrecord = Nstep / Anastep;
	std::vector<std::vector<particle_t>> swarms(Nswarm);
	std::vector<std::vector<particle_t>> records(Nswarm);
	// load init file
	io::loadinit();
	// load fef data file
	io::loaddat();
	// init swarms
	for(size_t i = 0; i < Nswarm; ++i){
		swarms.at(i) = std::vector<particle_t>(Ntraj);
		assign_initstate(swarms.at(i));
		records.at(i) = std::vector<particle_t>(static_cast<size_t>((Nrecord + 1) * Ntraj));
	}
#if _DEBUG >= 2
	cout << "debug: run_simulation: initialization finished, go! " << "\n";
#endif
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

#if _DEBUG >= 2
	cout << "debug: ";
#if defined(_OPENMP)
	cout << "thread " << omp_get_thread_num() << ": ";
#endif
	cout << "now on particle " << traj << "\n";
#endif
		while(step < Nstep){
			// store anastep data
			if(step % Anastep == 0){
				records[0][traj + irecord * Ntraj] = swarms[0][traj];
				//records[1][traj + irecord * Ntraj] = swarms[1][traj];
				//records[2][traj + irecord * Ntraj] = swarms[2][traj];
				++irecord;
			}
			// evolve
			CME(swarms[0][traj], traj);
			//BCME(swarms[1][traj], traj);
			//EF(swarms[2][traj], traj);
			++step;
		}
	}
#if _DEBUG >= 2
	cout << "debug: run_simulation: finished dynamics, now anal & output section ... " << "\n";
#endif
	for(irecord = 0; irecord < Nrecord; ++irecord){
	}
}
