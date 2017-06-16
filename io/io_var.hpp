#ifndef _SCATTER_IO_VAR_HPP
#define _SCATTER_IO_VAR_HPP
// variables in io 
#include <string>
#include "filemgr/filemgr.hpp"
#include "json_toolkit.hpp"

namespace scatter{
    namespace io{
        extern const std::string& jsonfile;
        extern const std::string& datfile;
        extern const std::string& initfile;
        extern const std::string& outfile;
		extern iomgr::filemgr_t filemgr;

		void load_var(const rapidjson::Document& doc);
		void print_var(void);
    };
};

#endif
