#ifndef _SCATTER_FILEIO_HPP
#define _SCATTER_FILEIO_HPP
// API module for file io in Scatter
#include "types.hpp"
#include "ioer.hpp"

namespace scatter{
	namespace io{
		void savedat(void);
		void loaddat(void);
		void saveinit(void);
		void loadinit(void);

		// API to save binary file
		template<typename ... Types>
			void save(const STRING_T path, const Types& ... x) 
			{
				ioer::output_t dest(path);
				dest.write(x ...);
			}

		// API to load binary file
		template<typename ... Types>
			void load(const STRING_T path, Types& ... x) 
			{
				ioer::input_t source(path);
				source.read(x ...);

			}
	};
};

#endif
