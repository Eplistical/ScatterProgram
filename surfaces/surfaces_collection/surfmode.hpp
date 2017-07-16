#ifndef _SURFMODE_HPP
#define _SURFMODE_HPP
// surfaces modes
#include "types.hpp"
#include "enumspace.hpp"

#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace enumspace {
	enum class surfmode_enum {
        CONSTANT,
        STEP,
        HARMONIC,
        FERMI,
        GAUSSIAN,
        NEWNS,
        MORSE,

		First = CONSTANT,
		Last = MORSE,
    };

    const boost::bimap<std::string, surfmode_enum> surfmode_dict = 
        boost::assign::list_of<boost::bimap<std::string, surfmode_enum>::relation>
            ("constant", surfmode_enum::CONSTANT)
            ("step", surfmode_enum::STEP)
            ("harmonic", surfmode_enum::HARMONIC)
            ("fermi", surfmode_enum::FERMI)
            ("gaussian", surfmode_enum::GAUSSIAN)
            ("newns", surfmode_enum::NEWNS)
            ("morse", surfmode_enum::MORSE)
            ;
};

#endif
