#include <iterator>
#include "types.hpp"
#include "vector.hpp"
#include "io.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "surfaces.hpp"
#include "run_var.hpp"
#include "analmode.hpp"
#include "anal.hpp"

using namespace scatter;

static std::vector<DOUBLE_T> _get_Ep(const particle_t ptcl) 
{
	std::vector<DOUBLE_T> rst(rem::dim);
	for (UINT_T d = 0; d < rem::dim; ++d) {
		rst[d] = surfaces_obj.fU(ptcl.surf, d, ptcl.r[d]);
	} 
	return rst;
}

static std::vector<DOUBLE_T> _get_Ek(const particle_t ptcl) 
{
	std::vector<DOUBLE_T> rst(rem::dim);
	for (UINT_T d = 0; d < rem::dim; ++d) {
		rst[d] = pow(ptcl.p[d], 2) / 2 / simulation::mass[d] ;
	} 
	return rst;
}


// API
std::vector<DOUBLE_T> scatter::extract_info(const particle_t& ptcl)
{
	std::vector<DOUBLE_T> rst;

	std::vector<DOUBLE_T> r = ptcl.r;
	std::vector<DOUBLE_T> p = ptcl.p;
	std::vector<DOUBLE_T>&& Ek = _get_Ek(ptcl);
	std::vector<DOUBLE_T>&& Ep = _get_Ep(ptcl);

	// rst: (surf, r, p, Ep, Ek)

	rst.push_back(static_cast<DOUBLE_T>(ptcl.surf));

	rst.insert(rst.end(), 
				std::make_move_iterator(r.begin()), 
				std::make_move_iterator(r.end())
				);

	rst.insert(rst.end(),
				std::make_move_iterator(r.begin()), 
				std::make_move_iterator(r.end())
				);

	rst.insert(rst.end(),
				std::make_move_iterator(Ep.begin()), 
				std::make_move_iterator(Ep.end())
				);

	rst.insert(rst.end(),
				std::make_move_iterator(Ek.begin()), 
				std::make_move_iterator(Ek.end())
				);

	return rst;
}
