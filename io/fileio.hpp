#ifndef _SCATTER_FILEIO_HPP
#define _SCATTER_FILEIO_HPP
// API module for file io in Scatter

#include <string>

namespace scatter{
	namespace io{
		void savedat(void);
		void loaddat(void);
		void saveinit(void);
		void loadinit(void);
		void loadinfile(void);
		void print_file(std::string file);
	};
};

#endif
