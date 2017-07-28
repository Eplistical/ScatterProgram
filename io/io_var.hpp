#ifndef _SCATTER_IO_VAR_HPP
#define _SCATTER_IO_VAR_HPP
// variables in io 
#include "scatter_basic.hpp"
#include "json_toolkit.hpp"
#include "ioer.hpp"

namespace scatter
{
    namespace io
	{
        extern STRING_T jsonfile;
        extern STRING_T datfile;
        extern STRING_T initfile;
        extern STRING_T outfile;
        extern STRING_T logfile;

        extern STRING_T arg_outfile;
        extern STRING_T arg_logfile;
        extern STRING_T arg_outdir;

		extern STRING_T indir;
		extern STRING_T outdir;

		extern ioer::output_t out_handler;
		extern ioer::output_t log_handler;

		VOID_T load_var(VOID_T);
		VOID_T print_var(VOID_T);
		VOID_T handler_init(VOID_T);
    };
};

#endif
