#ifndef _SURFACES_TYPES_HPP
#define _SURFACES_TYPES_HPP

#include <vector>
#include <memory>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include "packages/basesurf.hpp"

namespace surfaces{
    using _1d_surf_ptr_t = std::shared_ptr<surfaces::packages::basesurf>;

    class surf_info_t 
	{
		public:
			size_t mode;
			std::vector<double> para;

			surf_info_t& operator=(const surf_info_t& other){
				if(this != &other){
					mode = other.mode;
					para = other.para;
				}
			}

			std::vector<double> to_vec(void) const{
				std::vector<double> v;
				v.push_back(static_cast<double>(mode));
				v.insert(v.end(), para.begin(), para.end());
				return v;
			}
    };
};

#endif
