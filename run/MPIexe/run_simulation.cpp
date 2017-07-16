#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "types.hpp"
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include "timer.hpp"
#include "io.hpp"
#include "run.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"
#include "grid.hpp"
#include "surfaces.hpp"
#include "simulation.hpp"
#include "MPIer.hpp"
#include "MPI_helper.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;

// type for info recorder
template<typename ParamType>
	using InfoRecoderType = unordered_map<enumspace::dynamics_mode_enum, vector<ParamType> >;

using ParticleCollectionType = unordered_map<enumspace::dynamics_mode_enum, particle_t>;

VOID_T run_simulation(VOID_T)
{
	/* simulation run
	 */
	using namespace rem;
	using namespace grid;
	using namespace surfaces;
	using namespace simulation;

	// setup objs using paras 
	grid_obj = grid_t(rmin, rmax, Nr);
	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);

	// assign job
	std::vector<UINT_T> mybatch = MPIer::assign_job(simulation::Ntraj);
	std::vector<particle_t> final_states;
	UINT_T step;

	// load init & fef data
	if (MPIer::master) {
		io::loadinit();
		io::loaddat();
	}
	MPIer::bcast(0, simulation::r0p0);
	std::vector<DOUBLE_T>& fef = grid_obj.get_fef_ref();
	MPIer::bcast(0, fef);

	// basic para
	std::vector<UINT_T> job_info = mybatch;
	const UINT_T Nrecord = Nstep / Anastep + 1;
	std::vector<DOUBLE_T>&& tmp = std::vector<DOUBLE_T>();
	particle_t init_ptcl;

	// alogrithms to run
	std::vector<enumspace::dynamics_mode_enum> algorithms
	{
		enumspace::dynamics_mode_enum::CME,
		//enumspace::dynamics_mode_enum::BCME,
		//enumspace::dynamics_mode_enum::EF,
	};

	// show algorithms 
	if (MPIer::master) {
		out_handler.info_nonewline("algorithms: ");
		for (auto& it : algorithms) {
			out_handler.info_nonewline(enumspace::dynamics_mode_dict.right.at(it), ", ");
		}
		out_handler.newline();
	}

	// containers for info
	InfoRecoderType<DOUBLE_T> dyn_info;
	for (const auto& it : algorithms) {
		dyn_info[it] = std::vector<DOUBLE_T>();
	}

	// particle collections
	ParticleCollectionType ptcl;
	
	// -- do job! -- //
	for (UINT_T index : mybatch) {
		// load init state
		init_ptcl = particle_t(elestate);
		init_ptcl.ranforce.assign(dim, 0);
		init_ptcl.r = std::vector<DOUBLE_T>(r0p0.begin() + index * dim * 2, r0p0.begin() + index * dim * 2 + dim); 
		init_ptcl.p = std::vector<DOUBLE_T>(r0p0.begin() + index * dim * 2 + dim , r0p0.begin() + index * dim * 2 + dim * 2); 

		// initialize praticle for all algorithms
		for (const auto& it : algorithms) {
			ptcl[it] = init_ptcl;
		}
		step = 0;
		while (step < Nstep) {
			// loop over all its
			for (const auto& it : algorithms) {
				// record particle info 
				if (step % Anastep == 0) {
					tmp = ptcl[it].extract_info();
					dyn_info[it].insert(dyn_info[it].end(), tmp.begin(), tmp.end());
				}
				// evolve
				(*dynamic_algorithms[it])(ptcl[it], index);
			}
			++step;
		}
	}

	MPIer::barrier();

	// -- collect recorded info -- //
	vector<UINT_T> job_info_buf; 
	vector<DOUBLE_T> dyn_info_buf; 

	for (UINT_T r = 1; r < MPIer::size; ++r) {
		if (MPIer::rank == r) {
			MPIer::send(0, job_info);
			for (const auto& it : algorithms) {
				MPIer::send(0, dyn_info[it]);
			}
		}
		else if (MPIer::master) {
			MPIer::recv(r, job_info_buf);
			job_info.insert(job_info.end(), job_info_buf.begin(), job_info_buf.end());

			for (const auto& it : algorithms) {
				MPIer::recv(0, dyn_info_buf);
				dyn_info[it].insert(dyn_info[it].end(), dyn_info_buf.begin(), dyn_info_buf.end());
			}
		}
		MPIer::barrier();
	}

	// output
	if (MPIer::master) {
		STRING_T dyn_info_file = io::parent_dir + rem::jobname + STRING_T(".dyn_info.dat");
		out_handler.info_nonewline("saving particle dynamic info to ", dyn_info_file, "... ");
		UINT_T Nalgorithm = algorithms.size();
		UINT_T single_traj_info_size = dyn_info[algorithms[0]].size() / Ntraj;

		io::save_noclose( dyn_info_file, 
							Ntraj, dim, 
							Nalgorithm,
							single_traj_info_size,
							job_info);
		for (const auto& it : algorithms) {
			io::save_noclose(dyn_info_file, dyn_info[it]);
		}
		out_handler.info("done.");
	}
}

int main(int argc, char** argv)
{
	// MPI setup
	MPIer::setup();

	// parse infile 
	if(MPIer::master) {
		// parse infile 
		if(setup(argc, argv) != 0) return 0; 
		assert(rem::jobtype == "simulation");
	}

	// -- program begin -- //
	if (MPIer::master) {
		// parameter output
		out_handler.info("Program: scatter-simulation-MPI");
		out_handler.info(timer::now());
		timer::tic();
#if _DEBUG >= 1
		if (MPIer::master) log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
		out_handler.keyval()
			("infile", rem::infile) 
			("MPI-threadNum", MPIer::size)
			;
		rem::print_var();
		io::print_var();
		grid::print_var();
		surfaces::print_var();
		simulation::print_var();
	}

	// bcast vars
	scatter::bcast_vars();

	// run program
#if _DEBUG >= 1
	if (MPIer::master) log_handler.info( "debug: start running core part");
#endif

	run_simulation();

#if _DEBUG >= 1
	if (MPIer::master) log_handler.info( "debug: ending");
#endif

	// ending
	if(MPIer::master) {
		out_handler.info(timer::toc());
		out_handler.info(timer::now());
	}

	MPIer::finalize();
}
