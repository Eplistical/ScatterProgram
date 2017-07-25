#ifndef _SCATTER_ANAL_HPP
#define _SCATTER_ANAL_HPP
// module for data analysis
#include "scatter_basic.hpp"

#include <vector>
#include "simulation.hpp"

namespace scatter{
	std::vector<DOUBLE_T> extract_info(const particle_t& ptcl);
};

#endif
