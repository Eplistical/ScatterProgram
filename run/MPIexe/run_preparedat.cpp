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
	using scatter::io::savedat;

	// setup objs using paras 
#if _DEBUG >= 2
	if (mpier::master) log_handler.info( "debug: setting up grid_obj");
#endif

	grid_obj = grid_t(rmin, rmax, Nr);

#if _DEBUG >= 2
	if (mpier::master) log_handler.info( "debug: setting up surfaces_obj");
#endif

	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);

#if _DEBUG >= 2
	if (mpier::master) log_handler.info( "debug: assigning jobs");
#endif

	// assign job
	//std::vector<UINT_T> mybatch = mpier::assign_job_random(grid_obj.get_Ntot());
	std::vector<UINT_T> mybatch = mpier::assign_job(grid_obj.get_Ntot());

#if _DEBUG >= 2
	if (mpier::master) log_handler.info( "debug: allocating fef space");
#endif
	// allocate space for fef
	grid_obj.alloc_fef_space();
	
#if _DEBUG >= 2
	if (mpier::master) log_handler.info( "debug: looping");
#endif
	// do job!
	for (UINT_T index : mybatch) {
		// calc fef between |0> & |1>, the index^th element
		grid_obj.calc_fef(0, 1, index);
	}

	std::vector<DOUBLE_T> &fef = grid_obj.get_fef_ref();
	DOUBLE_T *tmp = NULL;
#if _DEBUG >= 2
	if (mpier::master) log_handler.info( "debug: collecting data");
#endif
	// reduce results, not using mpier here because the feature has not been added
	if (mpier::master)
		MPI::COMM_WORLD.Reduce(MPI_IN_PLACE, &fef[0], grid_obj.get_feflen(), MPI_DOUBLE, MPI_SUM, 0);
	else
		MPI::COMM_WORLD.Reduce(&fef[0], tmp, grid_obj.get_feflen(), MPI_DOUBLE, MPI_SUM, 0);

#if _DEBUG >= 2
	if (mpier::master) log_handler.info( "debug: saving results");
#endif
	// savedat
	if (mpier::master) 
		savedat();
}

int main(int argc, char** argv)
{
	// MPI setup
	mpier::setup();

	// parse infile 
	if(mpier::master) {
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
