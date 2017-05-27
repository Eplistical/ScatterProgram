#ifndef _SCATTER_INITMODE_HPP
#define _SCATTER_INITMODE_HPP
// init mode dict

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace scatter{
	namespace mode{
		enum class initmode {DELTA, RING, RP_GAUSSIAN, P_GAUSSIAN};
		const boost::bimap<std::string, initmode> init_mode_dict = 
			boost::assign::list_of<boost::bimap<std::string, initmode>::relation>
			("delta", initmode::DELTA)
			("ring", initmode::RING)
			("rp_gaussian", initmode::RP_GAUSSIAN)
			("p_gaussian", initmode::P_GAUSSIAN)
			;
	};
};
#endif
