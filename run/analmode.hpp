#ifndef _SCATTER_ANALMODE_HPP
#define _SCATTER_ANALMODE_HPP
// anal mode dict
#include "scatter_basic.hpp"
#include "enumspace.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace{
	enum class analmode_enum {
		dyn,
		final_info,
		hop_info,

		First = dyn,
		Last = hop_info
	};
};
#endif
