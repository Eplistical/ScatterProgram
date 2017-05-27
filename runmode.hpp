#ifndef _SCATTER_RUNMODE_HPP
#define _SCATTER_RUNMODE_HPP
// init mode dict

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace scatter{
	namespace mode{
		enum class runmode {TEST, SIMULATION, SURFACE};
		const boost::bimap<std::string, runmode> run_mode_dict = 
			boost::assign::list_of<boost::bimap<std::string, runmode>::relation>
			("test", runmode::TEST)
			("simulation", runmode::SIMULATION)
			("surface", runmode::SURFACE)
			;
	};
};
#endif
