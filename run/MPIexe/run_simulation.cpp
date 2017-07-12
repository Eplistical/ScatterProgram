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

void run_simulation(void)
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

	// do job!
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
		final_states.push_back(ptcl);
	}

	// collect final state
	
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
