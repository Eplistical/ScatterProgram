#ifndef _SCATTER_SIMULATION_VAR_HPP
#define _SCATTER_SIMULATION_VAR_HPP
// module for variables in simulation

#include <vector>

namespace scatter{
	namespace simulation{
		// simulation control parameters
		extern const std::vector<double>& mass;
		extern const std::vector<double>& omega;
		extern const size_t& Ntraj;
		extern const size_t& Nstep;
		extern const double& EndT;
		extern const double& dt;
	};
};

#endif
