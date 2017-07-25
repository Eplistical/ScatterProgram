#ifndef _SCATTER_PARTICLE_T_HPP
#define _SCATTER_PARTICLE_T_HPP
// module for single particle class
#include "scatter_basic.hpp"
#include "vector.hpp"

namespace scatter{
	class particle_t{
		public:
			std::vector<DOUBLE_T> r;
			std::vector<DOUBLE_T> p;
			std::vector<DOUBLE_T> ranforce;
			UINT_T surf;

			// big five
			explicit particle_t(VOID_T) : surf(0) {}
			explicit particle_t(UINT_T s) : surf(s) {}
			particle_t(const particle_t& other) = default; 
			particle_t& operator=(const particle_t& other) = default;
			~particle_t() = default;

			// setter
			VOID_T hop(UINT_T s) noexcept {surf = s;}
	};
};

#endif
