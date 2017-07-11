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
			size_t _Nsurf; 
			double _abs_gamma_threash;
			single_surf_t _gamma;
			std::vector<single_surf_t> _energy;
		public:
			// constructor and para initializer
			surfaces_t() = default;
			surfaces_t(const surfaces_t&) = default;
			surfaces_t& operator=(const surfaces_t&) = default;
			~surfaces_t() = default;
			explicit surfaces_t(size_t Nsurf);
			// setter
			void set_gamma(const std::vector<enumspace::surfmode_enum>& modes, const std::vector<std::vector<double> >& paras);
			void set_energy(const std::vector<enumspace::surfmode_enum>& modes, const std::vector<std::vector<double> >& paras);
			void set_abs_gamma_threash(double x);
			// getter
			size_t get_surf_number(void) const;
			single_surf_t get_gamma(void) const;
			single_surf_t get_energy(int i) const;
			std::vector<double> get_gamma_para(int d) const; 
			std::vector<double> get_energy_para(int i, int d) const; 
			std::vector<std::vector<double> > get_gamma_para_blank(void) const;
			std::vector<std::vector<double> > get_energy_para_blank(void) const;
			// U
			std::vector<double> vU(int i, const std::vector<double>& r) const; 		// energy as a vector @ surf i
			double fU(int i, const std::vector<double>& r) const;					// total energy @ surf i
			double fU(int i, int d, double x) const;								// energy @ surf i & dimension d
			// F
			std::vector<double> vdUdr(int i, const std::vector<double>& r) const;  	// nablaU
			std::vector<double> fF(int i, const std::vector<double>& r) const; 	   	// force
			double fF(int i, int d, double x) const;								// force @ surf i & dimension d
			// Gamma
			std::vector<double> vGamma(const std::vector<double>& r) const; 
			double fGamma(const std::vector<double>& r, double Gamma0) const;
			double fGamma(int d, double x) const;
			std::vector<double> vGammader(const std::vector<double>& r) const;
			std::vector<double> fGammader(const std::vector<double>& r, double Gamma0) const;
			double fGammader(int d, double x) const;
			bool small_gamma(const std::vector<double>& r, double Gamma0) const;
	};
};
#endif
