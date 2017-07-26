#ifndef _SCATTER_ANAL_HPP
#define _SCATTER_ANAL_HPP
// module for data analysis
#include "scatter_basic.hpp"
#include "vector.hpp"
#include "simulation.hpp"

namespace scatter{
	std::vector<DOUBLE_T> extract_info(const simulation::particle_t& ptcl);
	UINT_T get_dyn_info_piece_size(VOID_T);

	std::vector<DOUBLE_T> extract_info(const simulation::hop_t& hop);
	std::vector<DOUBLE_T> extract_info(const std::vector<simulation::hop_t>& hops);
	UINT_T get_hop_info_piece_size(VOID_T);
};

#endif
