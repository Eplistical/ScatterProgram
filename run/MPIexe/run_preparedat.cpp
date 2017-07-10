#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <algorithm>
#include <cassert>
#include "timer.hpp"
#include "io.hpp"
#include "run.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"
#include "grid.hpp"
#include "surfaces.hpp"
#include "mpier.hpp"
#include "MPI_helper.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;

void run_preparedat(void)
{
	/* preparedat run: prepare force & efric & fBCME 
	 * 	for given grid & surfaces.
	 * 	save data to datfile
	 *
	 */
	using namespace grid;
	using namespace surfaces;

	// setup objs using paras 
	grid_obj = grid_t(rmin, rmax, Nr);
	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);

	// create job index vector & shuffle
	std::vector<size_t> mybatch = mpier::assign_job_random(grid_obj.get_Ntot());
	
	// do job!
	
}

int main(int argc, char** argv)
{
	// MPI setup
	mpier::setup();

	// parse infile 
	if(mpier::master) {
		// parse infile 
		if(setup(argc, argv) != 0) return 0; 
		assert(rem::jobtype == "preparedat");
	}

	// -- program begin -- //
	if (mpier::master) {
		// parameter output
		out_handler.info("Program: scatter-preparedat");
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

	run_preparedat();

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
