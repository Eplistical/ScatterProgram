#include <iterator>
#include "scatter_basic.hpp"
#include "vector.hpp"
#include "io.hpp"
#include "rem.hpp"
#include "simulation.hpp"
#include "surfaces.hpp"
#include "run_var.hpp"
#include "analmode.hpp"
#include "anal.hpp"

using namespace scatter;
using scatter::simulation::particle_t;
using scatter::simulation::hop_t;

static std::vector<DOUBLE_T> _get_Ep(const particle_t& ptcl) 
{
	std::vector<DOUBLE_T> rst(rem::dim);
	for (UINT_T d = 0; d < rem::dim; ++d) {
		rst[d] = surfaces_obj.fU(ptcl.surf, d, ptcl.r[d]);
	} 
	return rst;
}

static std::vector<DOUBLE_T> _get_Ek(const particle_t& ptcl) 
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

	std::vector<DOUBLE_T>&& Ek = _get_Ek(ptcl);
	std::vector<DOUBLE_T>&& Ep = _get_Ep(ptcl);

	// rst: (surf, r, p, Ep, Ek)

	rst.push_back(static_cast<DOUBLE_T>(ptcl.surf));

	rst.insert(rst.end(), ptcl.r.begin(), ptcl.r.end()); 
	rst.insert(rst.end(), ptcl.p.begin(), ptcl.p.end()); 

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


UINT_T scatter::get_dyn_info_piece_size(VOID_T) noexcept
{
	return 1 + 4 * rem::dim;
}

std::vector<DOUBLE_T> scatter::extract_info(const hop_t& hop) noexcept
{
	std::vector<DOUBLE_T> rst;

	// rst: (traj_ID, from, to, r, p, gamma, energy_gap) 
	// 	[all cast to DOUBLE_T]
	rst.push_back(static_cast<DOUBLE_T>(hop.id));
	rst.push_back(static_cast<DOUBLE_T>(hop.from));
	rst.push_back(static_cast<DOUBLE_T>(hop.to));
	rst.insert(rst.end(), hop.r.begin(), hop.r.end());
	rst.insert(rst.end(), hop.p.begin(), hop.p.end());
	rst.push_back(hop.gamma);
	rst.push_back(hop.energy_gap);

	return rst;
}

std::vector<DOUBLE_T> scatter::extract_info(const std::vector<hop_t>& hops) noexcept
{
	std::vector<DOUBLE_T> tmp;
	std::vector<DOUBLE_T> rst;
	for (const auto& hop : hops) {
		tmp = extract_info(hop);
		rst.insert(rst.end(),
					std::make_move_iterator(tmp.begin()),
					std::make_move_iterator(tmp.end())
					);
	}

	return rst;
}

UINT_T scatter::get_hop_info_piece_size(VOID_T) noexcept
{
	return 5 + 2 * rem::dim;
}
