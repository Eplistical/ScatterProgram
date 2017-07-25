#ifndef _DYNAMICS_MODE_HPP
#define _DYNAMICS_MODE_HPP
// dynamic mode dict
#include "scatter_basic.hpp"
#include "enumspace.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace{
	enum class dynamic_mode_enum{
		CME,
		BCME,
		EF,

		First = CME,
		Last = EF, 
	};
	const boost::bimap<std::string, dynamic_mode_enum> dynamic_mode_dict = 
		boost::assign::list_of<boost::bimap<std::string, dynamic_mode_enum>::relation>
		("CME", dynamic_mode_enum::CME)
		("BCME", dynamic_mode_enum::BCME)
		("EF", dynamic_mode_enum::EF)
		;
};
#endif
