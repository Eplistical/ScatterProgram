#ifndef _SCATTER_IO_VAR_HPP
#define _SCATTER_IO_VAR_HPP
// variables in io 
#include <string>
#include "filemgr/filemgr.hpp"

namespace scatter{
    namespace io{
        extern const std::string& infile;
        extern const std::string& datfile;
        extern const std::string& initfile;
        extern const std::string& outfile;
		
		extern iomgr::filemgr_t filemgr;
    };
};

#endif
