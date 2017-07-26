#ifndef _SCATTER_ANAL_HPP
#define _SCATTER_ANAL_HPP
// module for data analysis
#include "scatter_basic.hpp"

#include <vector>
#include "simulation.hpp"

namespace scatter{
	std::vector<DOUBLE_T> extract_info(const simulation::particle_t& ptcl);
	std::vector<DOUBLE_T> extract_info(const simulation::hop_t& hop);
};

#endif
