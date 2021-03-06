#include "scatter_basic.hpp"
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

using simulation::algorithms;
using simulation::dynamic_algorithms;
using simulation::particle_t;
using simulation::hop_t;
using simulation::hop_recorder;

// type for info recorder
template<typename ParamType>
	using InfoRecoderType = unordered_map<enumspace::dynamic_mode_enum, vector<ParamType> >;

using ParticleCollectionType = unordered_map<enumspace::dynamic_mode_enum, particle_t>;

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
	grid_obj = grid_t(rmin, rmax, boundary_rmin, boundary_rmax, Nr);

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: setting up surfaces_obj");
#endif

	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_abs_gamma_threash(gamma_cutoff);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);

	// assign job
	std::vector<UINT_T> mybatch = MPIer::assign_job(simulation::Ntraj);
	std::vector<particle_t> final_states;
	UINT_T step;

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: loading init & dat file");
#endif

	// load init data
	if (MPIer::master) {
		try {
			io::loadinit();
		} catch (const scatter::FileNotOpenedError& e) {
			out_handler.info(e.what());
			out_handler.info("Please make sure you ran prepareinit before simulation.");
			MPIer::abort();
		}
	}

	MPIer::bcast(0, simulation::r0p0);

	// load fef data, use shared memory. 
	if (MPIer::master) {
		try {
			io::loaddat();
		} catch (const scatter::FileNotOpenedError& e) {
			out_handler.info(e.what());
			out_handler.info("Please make sure you ran preparedat before simulation.");
			MPIer::abort();
		}
	}

	if (MPIer::master)
		out_handler.info_nonewline("sending fef data to sub-commmunicator leaders ... ");

	// from master process, send data to all master_sm
	INT_T is_master_sm;
	for (UINT_T r = 1; r < MPIer::size; ++r) {
		if (MPIer::master) {
			MPIer::recv(r, is_master_sm);
			if (is_master_sm == 1) {

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: sending fef data to thread ", r);
#endif

				MPIer::send(r, grid_obj.get_fef_ref());
			}
		}
		else if (MPIer::rank == r) {
			is_master_sm = MPIer::master_sm ? 1 : 0;
			MPIer::send(0, is_master_sm);
			if (is_master_sm == 1) {
				MPIer::recv(0, grid_obj.get_fef_ref());
			}
		}
		MPIer::barrier();
	}


	if (MPIer::master) 
		out_handler.info( "done. ", timer::toc());

	// allocate shared memory in each sub-communicator, and set fef_data_ptr pointing to shared memory for all processes

	if (MPIer::master)
		out_handler.info_nonewline("Preparing shared memory ... ");

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: assigning fef_data_ptr");
#endif

	DOUBLE_T* fef_data_ptr;
	if (MPIer::master_sm) 
		fef_data_ptr = &grid_obj.get_fef_ref()[0];

	MPIer::make_sm(0, fef_data_ptr, grid_obj.get_feflen());
	grid_obj.set_fef_data_ptr(fef_data_ptr);
	
	MPIer::barrier();

	if (MPIer::master) 
		out_handler.info( "done. ", timer::toc());

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: setting up parameters");
#endif

	// basic para 
	const UINT_T Nrecord = Nstep / Anastep + 1;
	std::vector<DOUBLE_T>&& tmp = std::vector<DOUBLE_T>();
	particle_t init_ptcl;

	// containers for info
	InfoRecoderType<DOUBLE_T> dyn, hop;
	for (const auto& it : algorithms) {
		dyn[it] = std::vector<DOUBLE_T>();
		hop[it] = std::vector<DOUBLE_T>();
	}

	// particle collections
	ParticleCollectionType ptcl;

	// setup timer
	DOUBLE_T next_report_percent = 0.1;

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: start loop");
#endif

	MPIer::barrier();

	// -- do job! -- //
	UINT_T index;
	for (UINT_T i = 0, N = mybatch.size(); i < N; ++i) {
		index = mybatch[i];

#if _DEBUG >= 2
	if (MPIer::master) log_handler.info( "debug: propagating trajectory ", index);
#endif

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
					tmp = extract_info(ptcl.at(it));
					dyn.at(it).insert(dyn.at(it).end(), 
										std::make_move_iterator(tmp.begin()), 
										std::make_move_iterator(tmp.end())
										);
				}
				// evolve
				try {
					if (!grid_obj.is_leaving_boundary(ptcl.at(it).r, ptcl.at(it).p)) { 
						(*dynamic_algorithms.at(it))(ptcl.at(it), index);
					}
				} catch (const scatter::ScatterError& e) {
					std::cout 
						<< "thread " << MPIer::rank 
						<< ": catched ScatterError on traj " << index 
						<< " with algorithm " << enumspace::dynamic_mode_dict.right.at(it)
						<< ".\nerrmsg:"
						<< e.what() 
						<< "\n";
					MPIer::abort();
				}
			}
			++step;
		}

		// timer
		if (MPIer::master and ((i + 1) / static_cast<DOUBLE_T>(N)) >= next_report_percent) {
			out_handler.info(next_report_percent * 100, " \% done, ", timer::toc());
			next_report_percent += 0.1;
		}
	}

	// convert hop info
	for (const auto& it : algorithms) {
		if (hop_recorder.find(it) != hop_recorder.end()) {
			hop[it] = extract_info(hop_recorder.at(it));
		}
	}
	
	MPIer::barrier();

	// -- collect recorded data -- //
	if (MPIer::master) 
		out_handler.info_nonewline( "collecting dynamic data ... ");

	vector<UINT_T> mybatch_buf; 
	vector<DOUBLE_T> dyn_buf; 
	vector<DOUBLE_T> hop_buf;

	for (UINT_T r = 1; r < MPIer::size; ++r) {
		if (MPIer::rank == r) {
			MPIer::send(0, mybatch);
			for (const auto& it : algorithms) {
				MPIer::send(0, dyn.at(it));
				if (hop.find(it) != hop.end()) {
					MPIer::send(0, hop.at(it));
				}
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
				MPIer::recv(r, dyn_buf);
				dyn.at(it).insert(dyn.at(it).end(), 
									std::make_move_iterator(dyn_buf.begin()), 
									std::make_move_iterator(dyn_buf.end())
									);
				if (hop.find(it) != hop.end()) {
					MPIer::recv(r, hop_buf);
					hop.at(it).insert(hop.at(it).end(), 
										std::make_move_iterator(hop_buf.begin()), 
										std::make_move_iterator(hop_buf.end())
										);
				}
			}
		}
		MPIer::barrier();
	}

	if (MPIer::master) 
		out_handler.info( "done. ", timer::toc());

	// -- save dynamic data to <jobname>.dyn.dat -- //
	if (MPIer::master) 
		out_handler.info_nonewline( "saving dynamic data to ", io::outdir + rem::jobname + STRING_T(".dyn.dat ... "));

	if (MPIer::master) {
		STRING_T dyn_file = io::outdir + rem::jobname + STRING_T(".dyn.dat");
		UINT_T Nalgorithm = algorithms.size();
		UINT_T dyn_info_piece_size = get_dyn_info_piece_size();

		io::save_noclose( dyn_file, 
							dim, Ntraj,
							Nalgorithm,
							dyn_info_piece_size,
							mybatch);
		for (const auto& it : algorithms) {
			io::save_noclose(dyn_file, dyn.at(it));
		}

		out_handler.info("done. ", timer::toc());
	}

	// -- save hop data to <jobname>.hop.dat -- //
	if (MPIer::master) 
		out_handler.info_nonewline( "saving hop data to ", io::outdir + rem::jobname + STRING_T(".hop.dat ... "));

	if (MPIer::master) {
		STRING_T hop_file = io::outdir + rem::jobname + STRING_T(".hop.dat");
		std::vector<UINT_T> Nhops;
		UINT_T Nalgorithm = algorithms.size();
		UINT_T hop_info_piece_size = get_hop_info_piece_size();
		
		for (const auto& it : algorithms) {
			if (hop.find(it) != hop.end()) {
				Nhops.push_back(hop.at(it).size() / hop_info_piece_size);
			} 
			else {
				Nhops.push_back(0);
			}
		}

		io::save_noclose( hop_file, 
							dim, Ntraj,
							Nalgorithm,
							hop_info_piece_size,
							Nhops);

		for (const auto& it : algorithms) {
			if (hop.find(it) != hop.end()) {
				io::save_noclose(hop_file, hop.at(it));
			}
		}

		out_handler.info("done. ", timer::toc());
	}

	if (MPIer::master) 
		out_handler.info("done. ", timer::toc());

	// -- DONE -- //
}

int main(int argc, char** argv)
{
	// MPI setup
	MPIer::setup();
	MPIer::setup_sm();

	// parse infile 
	if(MPIer::master) {
		// parse infile 
		if(setup(argc, argv) != 0) MPIer::abort(); 
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
