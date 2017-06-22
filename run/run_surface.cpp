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

	for(size_t d = 0, N = rem::dim; d < N; ++d){
		mesh.get_grid(d);
	}
}
