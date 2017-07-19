#ifndef _SCATTER_ANALMODE_HPP
#define _SCATTER_ANALMODE_HPP
// anal mode dict
#include "types.hpp"
#include "enumspace.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace{
	enum class analmode_enum {
		dyn_info,
		final_info,
		hop_info,

		First = dyn_info,
		Last = hop_info
	};
};
#endif
