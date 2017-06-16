#ifndef _SCATTER_INITMODE_HPP
#define _SCATTER_INITMODE_HPP
// init mode dict

#include <string>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace{
	enum class initmode_enum {
		DELTA, 
		RING, 
		RP_GAUSSIAN,
		P_GAUSSIAN
	};
	const boost::bimap<std::string, initmode_enum> initmode_dict = 
		boost::assign::list_of<boost::bimap<std::string, initmode_enum>::relation>
		("delta", initmode_enum::DELTA)
		("ring", initmode_enum::RING)
		("rp_gaussian", initmode_enum::RP_GAUSSIAN)
		("p_gaussian", initmode_enum::P_GAUSSIAN)
		;
};
#endif
