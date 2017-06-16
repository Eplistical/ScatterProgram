#ifndef _SCATTER_REM_VAR_HPP
#define _SCATTER_REM_VAR_HPP
// API module for rem in Scatter
#include <string>
#include "json_toolkit.hpp"

namespace scatter{
	namespace rem{
		// args passed by command line
		extern const std::string& infile;
		extern const size_t& threadNum;
		// job control parameters
		extern const std::string& jobname;
		extern const std::string& jobtype;
		extern const bool& loaddat;
		// basic parameters for the system(useful both for structure & dynamics)
		extern const size_t& dim;
		extern const size_t& dim2;
		extern const double& hbar;
		extern const double& kT;
		extern const double& Gamma0;


		void load_var(const rapidjson::Document& doc);
		void print_var(void);
	};
};
#endif
