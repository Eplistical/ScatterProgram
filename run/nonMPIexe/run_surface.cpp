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

VOID_T run_surface(VOID_T){
	/* surface run: generate a file containning surfaces data
	 * 	that can be used to plot out surfaces.
	 *
	 */
	using namespace grid;
	using namespace surfaces;
	DOUBLE_T U, F, G, Gder;
	grid_obj = grid_t(rmin, rmax, boundary_rmin, boundary_rmax, Nr);
	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_abs_gamma_threash(gamma_cutoff);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);
	out_handler.info("surfaces:\n", "i^th surf & d^dimension");
	out_handler.tabout("x", "U(x)", "F(x)", "gamma(x)", "gamma'(x)");
	// loop output
	for(UINT_T d = 0; d < rem::dim; ++d){
		for(UINT_T i = 0; i < surfnum; ++i){
			out_handler.drawline('#');
			out_handler.info("d = ", d, "i = ", i);
			for(auto& x : grid_obj.get_grid(d)){
				U = surfaces_obj.fU(i, d, x);
				F = surfaces_obj.fF(i, d, x);
				G = surfaces_obj.fGamma(d, x);
				Gder = surfaces_obj.fGammader(d, x);
				out_handler.tabout(x, U, F, G, Gder);
			}
		}
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

	run_surface();

#if _DEBUG >= 1
	log_handler.info( "debug: ending");
#endif
	// ending
	out_handler.info(timer::toc());
	out_handler.info(timer::now());
}
