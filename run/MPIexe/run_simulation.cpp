#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include "types.hpp"
#include <algorithm>
#include <cassert>
#include "timer.hpp"
#include "io.hpp"
#include "run.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"
#include "grid.hpp"
#include "surfaces.hpp"
#include "simulation.hpp"
#include "mpier.hpp"
#include "MPI_helper.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;

VOID_T run_simulation(void)
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
	std::vector<UINT_T> mybatch = mpier::assign_job(simulation::Ntraj);
	std::vector<particle_t> final_states;
	UINT_T step;
	particle_t ptcl;

	// load init
	io::loadinit();

	// containers for final info
	std::vector<UINT_T> job_info = mybatch;
	std::vector<UINT_T> final_surf_info;
	std::vector<DOUBLE_T> final_r_info;
	std::vector<DOUBLE_T> final_p_info;

	// -- do job! -- //
	for (UINT_T index : mybatch) {
		// initialize praticle
		ptcl.surf = elestate;
		ptcl.ranforce.assign(dim, 0.0);
		ptcl.r = std::vector<DOUBLE_T>(r0p0.begin() + index * dim * 2, r0p0.begin() + index * dim * 2 + dim); 
		ptcl.p = std::vector<DOUBLE_T>(r0p0.begin() + index * dim * 2 + dim , r0p0.begin() + index * dim * 2 + dim * 2); 

		step = 0;
		while (step < Nstep) {
			// store anastep data
			if (step % Anastep == 0) {
			}
			// evolve
			CME(ptcl, index);
			++step;
		}
		// record final state
		final_surf_info.push_back(ptcl.surf);
		final_r_info.insert(final_r_info.end(), ptcl.r.begin(), ptcl.r.end());
		final_p_info.insert(final_p_info.end(), ptcl.p.begin(), ptcl.p.end());
	}

	mpier::barrier();

	// -- collect final info -- //
	if (not mpier::master) {
		mpier::send(0, mpier::rank * 100, 
						job_info, 
						final_surf_info, 
						final_r_info, 
						final_p_info);
	}
	else {
		vector<UINT_T> job_info_buf; 
		vector<UINT_T> surf_info_buf; 
		vector<DOUBLE_T> r_info_buf; 
		vector<DOUBLE_T> p_info_buf; 
		for (UINT_T r = 1; r < mpier::size; ++r) {
			mpier::recv(r, r * 100, 
							job_info_buf, 
							surf_info_buf, 
							r_info_buf, 
							p_info_buf);
			job_info.insert(job_info.end(), job_info_buf.begin(), job_info_buf.end());
			final_surf_info.insert(final_surf_info.end(), surf_info_buf.begin(), surf_info_buf.end());
			final_r_info.insert(final_r_info.end(), r_info_buf.begin(), r_info_buf.end());
			final_p_info.insert(final_p_info.end(), p_info_buf.begin(), p_info_buf.end());
		} 
	}

	// output
	STRING_T final_info_file = io::parent_dir + rem::jobname + STRING_T(".final.dat");
	out_handler.info_nonewline("saving final particle info to ", final_info_file, "... ");
	io::save( final_info_file, 
				Ntraj, dim, 
				job_info, 
				final_surf_info, 
				final_r_info, 
				final_p_info);
	out_handler.info("done.");
}

int main(int argc, char** argv)
{
	// MPI setup
	mpier::setup();

	// parse infile 
	if(mpier::master) {
		// parse infile 
		if(setup(argc, argv) != 0) return 0; 
		assert(rem::jobtype == "simulation");
	}

	// -- program begin -- //
	if (mpier::master) {
		// parameter output
		out_handler.info("Program: scatter-simulation-MPI");
		out_handler.info(timer::now());
		timer::tic();
#if _DEBUG >= 1
		if (mpier::master) log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
		out_handler.keyval()
			("infile", rem::infile) 
			("MPI-threadNum", mpier::size)
			;
		rem::print_var();
		io::print_var();
		grid::print_var();
		surfaces::print_var();
	}

	// bcast vars
	scatter::bcast_vars();

	// run program
#if _DEBUG >= 1
	if (mpier::master) log_handler.info( "debug: start running core part");
#endif

	run_simulation();

#if _DEBUG >= 1
	if (mpier::master) log_handler.info( "debug: ending");
#endif

	// ending
	if(mpier::master) {
		out_handler.info(timer::toc());
		out_handler.info(timer::now());
	}

	mpier::finalize();
}
