#ifndef _SCATTER_IO_VAR_HPP
#define _SCATTER_IO_VAR_HPP
// variables in io 
#include <string>
#include "json_toolkit.hpp"
#include "ioer.hpp"

namespace scatter{
    namespace io{
        extern std::string jsonfile;
        extern std::string datfile;
        extern std::string initfile;
        extern std::string outfile;
        extern std::string logfile;

		extern ioer::output_t out_handler;
		extern ioer::output_t log_handler;

		void load_var(void);
		void print_var(void);
		void handler_init(void);
    };
};

#endif
