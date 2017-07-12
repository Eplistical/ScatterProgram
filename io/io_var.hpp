#ifndef _SCATTER_IO_VAR_HPP
#define _SCATTER_IO_VAR_HPP
// variables in io 
#include "types.hpp"
#include "json_toolkit.hpp"
#include "ioer.hpp"

namespace scatter{
    namespace io{
        extern STRING_T jsonfile;
        extern STRING_T datfile;
        extern STRING_T initfile;
        extern STRING_T outfile;
        extern STRING_T logfile;

		extern ioer::output_t out_handler;
		extern ioer::output_t log_handler;

		void load_var(void);
		void print_var(void);
		void handler_init(void);
    };
};

#endif
