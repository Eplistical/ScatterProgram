#ifndef _SCATTER_PARTICLE_T_HPP
#define _SCATTER_PARTICLE_T_HPP
// module for single particle class
#include <vector>

namespace scatter{
	class particle_t{
		public:
			std::vector<double> r;
			std::vector<double> p;
			std::vector<double> ranforce;
			size_t surf;

			// big five
			explicit particle_t(void) : surf(0) {}
			explicit particle_t(size_t s) : surf(s) {}
			particle_t(const particle_t& other) = default; 
			particle_t& operator=(const particle_t& other) = default;
			~particle_t() = default;

			// setter
			void hop(size_t s){surf = s;}
	};
};

#endif
