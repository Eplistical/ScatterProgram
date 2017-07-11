#ifndef _SCATTER_REM_VAR_HPP
#define _SCATTER_REM_VAR_HPP
// API module for rem in Scatter
#include "scatter_basic.hpp"
#include <string>
#include "json_toolkit.hpp"

namespace scatter{
	namespace rem{
		// args passed by command line
		extern std::string infile;
		extern size_t threadNum;
		// job control parameters
		extern std::string jobname;
		extern std::string jobtype;
		extern bool loaddat;
		// basic parameters for the system(useful both for structure & dynamics)
		extern size_t dim;
		extern size_t dim2;
		extern double hbar;
		extern double kT;
		extern double Gamma0;


		void load_var(const rapidjson::Document& doc);
		void print_var(void);
	};
};
#endif
