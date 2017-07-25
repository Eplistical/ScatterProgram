#include "scatter_basic.hpp"
#include <iostream>
#include <cassert>
#include "timer.hpp"
#include "io.hpp"
#include "run.hpp"
#include "simulation.hpp"
#include "vars.hpp"
#include "json_toolkit.hpp"

using scatter::io::out_handler;
using scatter::io::log_handler;

using namespace std;
using namespace scatter;

VOID_T run_test(VOID_T){
	/* 
	 * for test some functions
	 *
	 */
	using namespace grid;
	grid_obj = grid_t(rmin, rmax, boundary_rmin, boundary_rmax, Nr);
	std::vector<DOUBLE_T> r;
	UINT_T k;
	for (UINT_T i = 0; i < grid_obj.get_Ntot(); ++i) {
		r = grid_obj.index_to_r(i);
		k = grid_obj.r_to_index(r);
		if(i != k)
			std::cout << i << " => " << r[0] << "\t" << r[1] << " =>> "
				<< k  <<"\t" << (i == k) 
				<< "\n" ;
	}

}

int main(int argc, char** argv){
	if(setup(argc, argv) != 0) return 0; 
	// -- program begin -- //
	out_handler.info(timer::now());
	timer::tic();
#if _DEBUG >= 1
	log_handler.info( "debug: debug level ", _DEBUG, "\n");
#endif
	// parameter output
	out_handler.info("Program: scatter-surface");
	out_handler.keyval()
		("infile", rem::infile) 
		("threadNum", rem::threadNum)
		;
	rem::print_var();
	io::print_var();
	grid::print_var();
	surfaces::print_var();
	// run program
#if _DEBUG >= 1
	log_handler.info( "debug: start running core part");
#endif

	run_test();

#if _DEBUG >= 1
	log_handler.info( "debug: ending");
#endif
	// ending
	out_handler.info(timer::toc());
	out_handler.info(timer::now());
}
