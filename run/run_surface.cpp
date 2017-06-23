#include <iostream>
#include "vector.hpp"
#include "rem.hpp"
#include "surfaces.hpp"
#include "grid.hpp"
#include "run.hpp"
#include "ioer.hpp"

using namespace scatter;

void scatter::run_surface(void){
	/* surface run: generate a file containning surfaces data
	 * 	that can be used to plot out surfaces.
	 *
	 */
	using namespace ioer;
	using namespace grid;
	using namespace surfaces;
	grid_t mesh(rmin, rmax, Nr);
	surfaces_t surf(surfnum);
	surf.set_gamma(gammamode, gammapara);
	surf.set_energy(surfmode, surfpara);

	for(size_t i = 0; i < surfnum; ++i){
		for(size_t d = 0, Ndim = rem::dim; d < Ndim; ++d){
			ioer::info("i = ", i, "d = ", d);
			for(auto& x : mesh.get_grid(d)){
				ioer::tabout(x, surf.fU(i, d, x));
			}
		}
	}
}
