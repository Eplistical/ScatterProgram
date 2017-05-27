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

			// constructor
			explicit particle_t(void) : surf(0){}
			explicit particle_t(size_t s) : surf(s){}

			// setter
			void hop(size_t s){surf = s;}

			// operator
			particle_t& operator=(const particle_t& other){
				if(this != &other){
					r = other.r;
					p = other.p;
					surf = other.surf;
				}
				return *this;
			}
	};
};

#endif
