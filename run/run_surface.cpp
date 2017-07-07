#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <iostream>
#include "vector.hpp"
#include "rem.hpp"
#include "surfaces.hpp"
#include "grid.hpp"
#include "run.hpp"
#include "io.hpp"

using scatter::io::out_handler;
using namespace scatter;

void scatter::run_surface(void){
	/* surface run: generate a file containning surfaces data
	 * 	that can be used to plot out surfaces.
	 *
	 */
	using namespace ioer;
	using namespace grid;
	using namespace surfaces;
	grid_obj = grid_t(rmin, rmax, Nr);
	surfaces_obj = surfaces_t(surfnum);
	surfaces_obj.set_gamma(gammamode, gammapara);
	surfaces_obj.set_energy(surfmode, surfpara);
	out_handler.info("surfaces:\n", "i^th surf & d^dimension");
	out_handler.tabout("x", "U(x)", "F(x)", "gamma(x)", "gamma'(x)");
	// loop output
	for(size_t i = 0; i < surfnum; ++i){
		for(size_t d = 0, Ndim = rem::dim; d < Ndim; ++d){
			out_handler.drawline('#');
			out_handler.info("i = ", i, "d = ", d);
			for(auto& x : grid_obj.get_grid(d)){
				out_handler.tabout(x, surfaces_obj.fU(i, d, x), surfaces_obj.fF(i, d, x), 
								surfaces_obj.fGamma(d, x), surfaces_obj.fGammader(d, x));
			}
		}
	}
}
