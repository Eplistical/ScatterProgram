#ifndef _SCATTER_IO_VAR_HPP
#define _SCATTER_IO_VAR_HPP
// variables in io 
#include <string>
#include "filemgr/filemgr.hpp"
#include "json_toolkit.hpp"

namespace scatter{
    namespace io{
        extern std::string jsonfile;
        extern std::string datfile;
        extern std::string initfile;
        extern std::string outfile;
		extern iomgr::filemgr_t filemgr;

		void load_var(void);
		void print_var(void);
    };
};

#endif
