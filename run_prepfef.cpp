#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <cassert>
#include "timer.hpp"
#include "io.hpp"
#include "run.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"
#include "mpier.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;


int main(int argc, char** argv){
	// MPI setup
	mpier::setup();

	// parse infile 
	if(mpier::master) {
		// parse infile 
		if(setup(argc, argv) != 0) return 0; 
		assert(rem::jobtype == "prepfef");
		// parse arg again to override infile
		arg_parser(argc, argv);
		// assign output/log destination
		io::handler_init();
	}

	// -- program begin -- //
	if (mpier::master) {
		out_handler.info("Program: scatter-prepfef");
		out_handler.info(timer::now());
		timer::tic();
#if _DEBUG >= 1
		log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
		out_handler.keyval()
			("infile", rem::infile) 
			("MPI-threadNum", mpier::size)
			;
	}

	// run program
	mpier::bcast(0, rem::kT, rem::jobname, grid::rmin);
	if(mpier::master) {
		out_handler.info(rem::kT, rem::jobname, grid::rmin);
	}
	
	// ending
	if(mpier::master) {
		out_handler.info(timer::toc());
		out_handler.info(timer::now());
	}

	mpier::finalize();
}
