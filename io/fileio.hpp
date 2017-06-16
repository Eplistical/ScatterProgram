#ifndef _SCATTER_FILEIO_HPP
#define _SCATTER_FILEIO_HPP
// API module for file io in Scatter

#include "filemgr/filemgr.hpp"
#include "surfaces.hpp"
#include "grid.hpp"
#include "simulation.hpp"

namespace scatter{
	namespace io{
		void savedat(const scatter::surfaces_t& surf, scatter::grid_t& grid);
		void loaddat(const scatter::surfaces_t& surf, scatter::grid_t& grid);
		void saveinit(void);
		void loadinit(void);
		void loadinfile(void);
		void print_file(std::string file);
	};
};

#endif
