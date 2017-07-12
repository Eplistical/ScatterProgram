#ifndef _SCATTER_GRID_T_HPP
#define _SCATTER_GRID_T_HPP
// module for grid_t
#include "types.hpp"

#include <vector>

namespace scatter{
	class grid_t{
		private:
			// data 
			std::vector<DOUBLE_T> _rmin;
			std::vector<DOUBLE_T> _rmax;
			std::vector<DOUBLE_T> _dr;
			std::vector<UINT_T> _Nr;
			std::vector<DOUBLE_T> _fef;
			UINT_T _forcelen;
			UINT_T _efriclen;
			UINT_T _fBCMElen;
			UINT_T _forceoffset;
			UINT_T _efricoffset;
			UINT_T _fBCMEoffset;

			// program-dependent 
			UINT_T r_to_index_filtered(const std::vector<DOUBLE_T>& r) const;
		public:
			// constructor
			grid_t() = default;
			~grid_t() = default;
			grid_t(const grid_t&) = default;
			grid_t& operator=(const grid_t&) = default;
			explicit grid_t(const std::vector<DOUBLE_T>& rmin, const std::vector<DOUBLE_T>& rmax, const std::vector<UINT_T>& Nr);
			// setter
			std::vector<DOUBLE_T>& get_fef_ref(void);
			void alloc_fef_space(void);
			void calc_fef(UINT_T S0, UINT_T S1, UINT_T index);
			// getter
			std::vector<DOUBLE_T> get_grid(UINT_T d) const;
			std::vector<DOUBLE_T> get_rmin(void) const;
			std::vector<DOUBLE_T> get_rmax(void) const;
			std::vector<DOUBLE_T> get_dr(void) const;
			std::vector<UINT_T> get_Nr(void) const;
			DOUBLE_T get_rmin(UINT_T d) const;
			DOUBLE_T get_rmax(UINT_T d) const;
			DOUBLE_T get_dr(UINT_T d) const;
			UINT_T get_Nr(UINT_T d) const;
			UINT_T get_Ntot(void) const;
			UINT_T get_forcelen(void) const;
			UINT_T get_efriclen(void) const;
			UINT_T get_fBCMElen(void) const;
			UINT_T get_feflen(void) const;
			std::vector<DOUBLE_T> get_force(const std::vector<DOUBLE_T>& r) const;
			std::vector<DOUBLE_T> get_efric(const std::vector<DOUBLE_T>& r) const;
			std::vector<DOUBLE_T> get_fBCME(const std::vector<DOUBLE_T>& r) const;
			UINT_T r_to_index(const std::vector<DOUBLE_T>& r) const;
			std::vector<DOUBLE_T> index_to_r(UINT_T index) const;
	};
};
#endif
