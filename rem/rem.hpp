#ifndef _SCATTER_REM_HPP
#define _SCATTER_REM_HPP
// API module for rem in Scatter
#include <string>

namespace scatter{
	namespace rem{
		// job control parameters
		extern const std::string& jobname;
		extern const bool& loaddat;
		extern const size_t& threadNum;
		// basic parameters for the system(useful both for structure & dynamics)
		extern const size_t& dim;
		extern const size_t& dim2;
		extern const double& hbar;
		extern const double& kT;
		extern const double& Gamma0;
	};
};
#endif
