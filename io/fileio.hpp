#ifndef _SCATTER_FILEIO_HPP
#define _SCATTER_FILEIO_HPP
// API module for file io in Scatter
#include "types.hpp"
#include "ioer.hpp"

namespace scatter{
	namespace io{
		VOID_T savedat(VOID_T);
		VOID_T loaddat(VOID_T);
		VOID_T saveinit(VOID_T);
		VOID_T loadinit(VOID_T);

		// API to save binary file
		template<typename ... Types>
			VOID_T save(const STRING_T path, const Types& ... x) 
			{
				ioer::output_t dest(path);
				dest.write(x ...);
			}

		// API to load binary file
		template<typename ... Types>
			VOID_T load(const STRING_T path, Types& ... x) 
			{
				ioer::input_t source(path);
				source.read(x ...);

			}
	};
};

#endif
