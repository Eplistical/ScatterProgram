#ifndef _SCATTER_SURF_T_HPP
#define _SCATTER_SURF_T_HPP
// API for surfaces module
#include <vector>
#include "types.hpp"
#include "single_surf.hpp"

using surfaces::single_surf_t;
using surfaces::surf_info_t;

namespace scatter{
	class surf_t{
		private:
			size_t _Nsurf; 
			double _abs_gamma_threash;
			single_surf_t _gamma;
			std::vector<single_surf_t> _energy;
		public:
			// constructor and para initializer
			explicit surf_t();
			explicit surf_t(size_t Nsurf);
			// setter
			void set_gamma(const std::vector<surf_info_t>& info);
			void set_energy(size_t i, const std::vector<surf_info_t>& info);
			void set_abs_gamma_threash(double x);
			// getter
			size_t get_surf_number(void) const;
			single_surf_t get_gamma(void) const;
			single_surf_t get_energy(int i) const;
			std::vector<double> get_gamma_para(int d) const; 
			std::vector<double> get_energy_para(int i, int d) const; 
			// U
			std::vector<double> vU(int i, const std::vector<double>& r) const; 
			std::vector<double> vU0(const std::vector<double>& r) const; 
			std::vector<double> vU1(const std::vector<double>& r) const;
			double fU(int i, const std::vector<double>& r) const;
			double fU0(const std::vector<double>& r) const ;
			double fU1(const std::vector<double>& r) const ;
			// F
			std::vector<double> vdUdr(int i, const std::vector<double>& r) const; 
			std::vector<double> vF(int i, const std::vector<double>& r) const; 
			std::vector<double> fF0(const std::vector<double>& r) const; 
			std::vector<double> fF1(const std::vector<double>& r) const; 
			// Gamma
			std::vector<double> vGamma(const std::vector<double>& r) const; 
			double fGamma(const std::vector<double>& r, double Gamma0) const;
			std::vector<double> vGammader(const std::vector<double>& r) const;
			std::vector<double> fGammader(const std::vector<double>& r, double Gamma0) const;
			bool small_gamma(const std::vector<double>& r, double Gamma0) const;
			// calc fef
			void fef(const std::vector<double>& r, double *force, double *efric, double *fBCME);
	};
};
#endif
