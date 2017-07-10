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

	// assign job
	//std::vector<size_t> mybatch = mpier::assign_job_random(grid_obj.get_Ntot());
	std::vector<size_t> mybatch = mpier::assign_job(grid_obj.get_Ntot());

	// allocate space for fef
	grid_obj.alloc_fef_space();
	
	// do job!
	for (size_t index : mybatch) {
		// calc fef between |0> & |1>, the index^th element
		grid_obj.calc_fef(0, 1, index);
	}

	std::vector<double> &fef = grid_obj.get_fef_ref();
	double *tmp = NULL;
	// reduce results, not using mpier here because the feature has not been added
	if (mpier::master)
		MPI::COMM_WORLD.Reduce(MPI_IN_PLACE, &fef[0], grid_obj.get_feflen(), MPI_DOUBLE, MPI_SUM, 0);
	else
		MPI::COMM_WORLD.Reduce(&fef[0], tmp, grid_obj.get_feflen(), MPI_DOUBLE, MPI_SUM, 0);

	if (mpier::master) 
		out_handler.info("reduced fef:\n", fef);
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
