#ifndef _SURFACES_PACKAGE_CONTENTS_HPP
#define _SURFACES_PACKAGE_CONTENTS_HPP

#include <vector>
#include <memory>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include "basesurf.hpp"

namespace surfaces{
namespace packages{
    enum{
        CONSTANT,
        STEP,
        HARMONIC,
        FERMI,
        GAUSSIAN,
        NEWNS,
        MORSE,
    };

    const boost::bimap<std::string, int> dict = 
        boost::assign::list_of<boost::bimap<std::string, int>::relation>
            ("constant", CONSTANT)
            ("step", STEP)
            ("harmonic", HARMONIC)
            ("fermi", FERMI)
            ("gaussian", GAUSSIAN)
            ("newns", NEWNS)
            ("morse", MORSE)
            ;

    struct surf_para_package{
        size_t mode;
        std::vector<double> para;

        surf_para_package& operator=(const surf_para_package& other) {
            if(this != &other){
                mode = other.mode;
                para = other.para;
            }
        }
    };
};
};

#endif
