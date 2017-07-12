#ifndef _SCATTER_PARTICLE_T_HPP
#define _SCATTER_PARTICLE_T_HPP
// module for single particle class
#include "types.hpp"
#include <vector>

namespace scatter{
	class particle_t{
		public:
			std::vector<DOUBLE_T> r;
			std::vector<DOUBLE_T> p;
			std::vector<DOUBLE_T> ranforce;
			UINT_T surf;

			// big five
			explicit particle_t(void) : surf(0) {}
			explicit particle_t(UINT_T s) : surf(s) {}
			particle_t(const particle_t& other) = default; 
			particle_t& operator=(const particle_t& other) = default;
			~particle_t() = default;

			// setter
			void hop(UINT_T s){surf = s;}
	};
};

#endif
