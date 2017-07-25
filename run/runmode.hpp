#ifndef _SCATTER_RUNMODE_HPP
#define _SCATTER_RUNMODE_HPP
// run mode dict
#include "scatter_basic.hpp"
#include "enumspace.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace{
	enum class runmode_enum {
		SIMULATION, 
		SURFACE,
		PREPAREINIT,
		PREPAREDAT,

		First = SIMULATION,
		Last = PREPAREDAT,

	};
	const boost::bimap<std::string, runmode_enum> runmode_dict = 
		boost::assign::list_of<boost::bimap<std::string, runmode_enum>::relation>
		("simulation", runmode_enum::SIMULATION)
		("surface", runmode_enum::SURFACE)
		("prepareinit", runmode_enum::PREPAREINIT)
		("preparedat", runmode_enum::PREPAREDAT)
		;
};
#endif
