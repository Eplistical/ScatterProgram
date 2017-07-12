#ifndef _SCATTER_SURFACES_T_HPP
#define _SCATTER_SURFACES_T_HPP
// API for surfaces_collection module
#include "types.hpp"
#include <vector>
#include "surfaces_collection.hpp"


using surfaces_collection::single_surf_t;

namespace scatter{
	class surfaces_t{
		private:
			UINT_T _Nsurf; 
			DOUBLE_T _abs_gamma_threash;
			single_surf_t _gamma;
			std::vector<single_surf_t> _energy;
		public:
			// constructor and para initializer
			surfaces_t() = default;
			surfaces_t(const surfaces_t&) = default;
			surfaces_t& operator=(const surfaces_t&) = default;
			~surfaces_t() = default;
			explicit surfaces_t(UINT_T Nsurf);
			// setter
			void set_gamma(const std::vector<enumspace::surfmode_enum>& modes, const std::vector<std::vector<DOUBLE_T> >& paras);
			void set_energy(const std::vector<enumspace::surfmode_enum>& modes, const std::vector<std::vector<DOUBLE_T> >& paras);
			void set_abs_gamma_threash(DOUBLE_T x);
			// getter
			UINT_T get_surf_number(void) const;
			single_surf_t get_gamma(void) const;
			single_surf_t get_energy(INT_T i) const;
			std::vector<DOUBLE_T> get_gamma_para(INT_T d) const; 
			std::vector<DOUBLE_T> get_energy_para(INT_T i, INT_T d) const; 
			std::vector<std::vector<DOUBLE_T> > get_gamma_para_blank(void) const;
			std::vector<std::vector<DOUBLE_T> > get_energy_para_blank(void) const;
			// U
			std::vector<DOUBLE_T> vU(INT_T i, const std::vector<DOUBLE_T>& r) const; 		// energy as a vector @ surf i
			DOUBLE_T fU(INT_T i, const std::vector<DOUBLE_T>& r) const;					// total energy @ surf i
			DOUBLE_T fU(INT_T i, INT_T d, DOUBLE_T x) const;								// energy @ surf i & dimension d
			// F
			std::vector<DOUBLE_T> vdUdr(INT_T i, const std::vector<DOUBLE_T>& r) const;  	// nablaU
			std::vector<DOUBLE_T> fF(INT_T i, const std::vector<DOUBLE_T>& r) const; 	   	// force
			DOUBLE_T fF(INT_T i, INT_T d, DOUBLE_T x) const;								// force @ surf i & dimension d
			// Gamma
			std::vector<DOUBLE_T> vGamma(const std::vector<DOUBLE_T>& r) const; 
			DOUBLE_T fGamma(const std::vector<DOUBLE_T>& r, DOUBLE_T Gamma0) const;
			DOUBLE_T fGamma(INT_T d, DOUBLE_T x) const;
			std::vector<DOUBLE_T> vGammader(const std::vector<DOUBLE_T>& r) const;
			std::vector<DOUBLE_T> fGammader(const std::vector<DOUBLE_T>& r, DOUBLE_T Gamma0) const;
			DOUBLE_T fGammader(INT_T d, DOUBLE_T x) const;
			BOOL_T small_gamma(const std::vector<DOUBLE_T>& r, DOUBLE_T Gamma0) const;
	};
};
#endif
