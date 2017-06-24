#ifndef _SCATTER_ANALMODE_HPP
#define _SCATTER_ANALMODE_HPP
// anal mode dict

#include <string>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace{
	enum class analmode_enum {
		SIMU_ANAL_CME, 
		SIMU_ANAL_BCME, 
		SIMU_ANAL_EF, 

		FINAL_ANAL_CME, 
		FINAL_ANAL_BCME, 
		FINAL_ANAL_EF,

		HOP_ANAL_CME, 
		HOP_ANAL_BCME, 
	};
};
#endif
