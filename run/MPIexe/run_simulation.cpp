#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "types.hpp"
#include <algorithm>
#include <iterator>
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
#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: setting up grid_obj");
#endif
	grid_obj = grid_t(rmin, rmax, Nr);

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: setting up surfaces_obj");
#endif

	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);

	// assign job
	std::vector<UINT_T> mybatch = MPIer::assign_job(simulation::Ntraj);
	std::vector<particle_t> final_states;
	UINT_T step;

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: loading init & dat file");
#endif

	// load init & fef data
	if (MPIer::master) {
		io::loadinit();
		io::loaddat();
	}
	MPIer::bcast(0, simulation::r0p0);

	/*
	// ATTENTION: LARGE MEMORY REQUIREMENT
	std::vector<DOUBLE_T>& fef = grid_obj.get_fef_ref();
	MPIer::bcast(0, fef);
	*/
	// use shared memory to store fef, in order to reduce memory usage
	DOUBLE_T* fef_data_ptr;
	if (MPIer::master) fef_data_ptr = &grid_obj.get_fef_ref()[0];
	MPIer::make_sm(0, fef_data_ptr, grid_obj.get_feflen());
	grid_obj.set_fef_data_ptr(fef_data_ptr);

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: setting up parameters");
#endif

	// basic para
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

	// setup timer
	DOUBLE_T next_report_percent = 0.1;
	if (MPIer::master) timer::tic(99);

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: start loop");
#endif

	// -- do job! -- //
	UINT_T index;
	for (UINT_T i = 0, N = mybatch.size(); i < N; ++i) {
		index = mybatch[i];

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
					dyn_info[it].insert(dyn_info[it].end(), 
										std::make_move_iterator(tmp.begin()), 
										std::make_move_iterator(tmp.end())
										);
				}
				// evolve
				(*dynamic_algorithms[it])(ptcl[it], index);
			}
			++step;
		}
		// timer
		if (MPIer::master and ((i + 1) / static_cast<DOUBLE_T>(N)) >= next_report_percent) {
			out_handler.info(next_report_percent * 100, " \% Done, ", timer::toc(99));
			next_report_percent += 0.1;
		}
	}

	MPIer::barrier();

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: collecting data");
#endif

	// -- collect recorded data -- //
	vector<UINT_T> mybatch_buf; 
	vector<DOUBLE_T> dyn_info_buf; 

	for (UINT_T r = 1; r < MPIer::size; ++r) {
		if (MPIer::rank == r) {
			MPIer::send(0, mybatch);
			for (const auto& it : algorithms) {
				MPIer::send(0, dyn_info[it]);
			}
		}
		else if (MPIer::master) {

#if _DEBUG >= 2
			if (MPIer::master) log_handler.info( "debug: receiving data from thread ", r);
#endif
			MPIer::recv(r, mybatch_buf);
			mybatch.insert(mybatch.end(), 
							std::make_move_iterator(mybatch_buf.begin()), 
							std::make_move_iterator(mybatch_buf.end())
							);

			for (const auto& it : algorithms) {
				MPIer::recv(r, dyn_info_buf);
				dyn_info[it].insert(dyn_info[it].end(), 
									std::make_move_iterator(dyn_info_buf.begin()), 
									std::make_move_iterator(dyn_info_buf.end())
									);
			}
		}
		MPIer::barrier();
	}

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: saving dyn_info");
#endif

	// output
	if (MPIer::master) {
		STRING_T dyn_info_file = io::outdir + rem::jobname + STRING_T(".dyn_info.dat");
		out_handler.info_nonewline("saving particle dynamic info to ", dyn_info_file, "... ");
		UINT_T Nalgorithm = algorithms.size();
		UINT_T single_traj_info_size = dyn_info[algorithms[0]].size() / Ntraj;

		io::save_noclose( dyn_info_file, 
							Ntraj, dim, 
							Nalgorithm,
							single_traj_info_size,
							mybatch);
		for (const auto& it : algorithms) {
			io::save_noclose(dyn_info_file, dyn_info[it]);
		}
		out_handler.info("done.");
	}

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: run_simulation done");
#endif

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
