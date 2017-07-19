#ifndef _SCATTER_REM_VAR_HPP
#define _SCATTER_REM_VAR_HPP
// API module for rem in Scatter
#include "types.hpp"
#include "json_toolkit.hpp"

namespace scatter{
	namespace rem{
		// args passed by command line
		extern STRING_T infile;
		extern UINT_T threadNum;
		// job control parameters
		extern STRING_T jobname;
		extern BOOL_T loaddat;
		// basic parameters for the system(useful both for structure & dynamics)
		extern UINT_T dim;
		extern UINT_T dim2;
		extern DOUBLE_T hbar;
		extern DOUBLE_T kT;
		extern DOUBLE_T Gamma0;


		VOID_T load_var(const rapidjson::Document& doc);
		VOID_T print_var(VOID_T);
	};
};
#endif
