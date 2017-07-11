#ifndef _DYNAMICS_MODE_HPP
#define _DYNAMICS_MODE_HPP
// dynamics mode dict
#include "types.hpp"

#include <string>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace{
	enum class dynamics_mode_enum{
		CME,
		BCME,
		EF,
	};
	const boost::bimap<std::string, dynamics_mode_enum> dynamics_mode_dict = 
		boost::assign::list_of<boost::bimap<std::string, dynamics_mode_enum>::relation>
		("CME", dynamics_mode_enum::CME)
		("BCME", dynamics_mode_enum::BCME)
		("EF", dynamics_mode_enum::EF)
		;
};
#endif
