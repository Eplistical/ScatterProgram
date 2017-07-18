#ifndef _SCATTER_GRID_T_HPP
#define _SCATTER_GRID_T_HPP
// module for grid_t
#include "types.hpp"
#include <vector>

namespace scatter{
	class grid_t{
		protected:
			// data 
			std::vector<DOUBLE_T> _rmin;
			std::vector<DOUBLE_T> _rmax;
			std::vector<DOUBLE_T> _dr;
			std::vector<UINT_T> _Nr;
			std::vector<DOUBLE_T> _fef;

			// careful w/ raw pointer
			DOUBLE_T* _fef_data_ptr;

			UINT_T _forcelen;
			UINT_T _efriclen;
			UINT_T _fBCMElen;
			UINT_T _forceoffset;
			UINT_T _efricoffset;
			UINT_T _fBCMEoffset;
		public:
			// -- ctor/dtor/copy/move -- //
			explicit grid_t(const std::vector<DOUBLE_T>& rmin, const std::vector<DOUBLE_T>& rmax, const std::vector<UINT_T>& Nr);
			grid_t() = default;
			~grid_t() = default;
			grid_t(const grid_t&) = default;
			grid_t& operator=(const grid_t&) = default;
			grid_t(grid_t&&) = default;
			grid_t& operator=(grid_t&&) = default;

			// -- setters --//
			std::vector<DOUBLE_T>& get_fef_ref(VOID_T);
			DOUBLE_T* get_fef_data_ptr(VOID_T);

			VOID_T alloc_fef_space(VOID_T);
			VOID_T calc_fef(UINT_T S0, UINT_T S1, UINT_T index, DOUBLE_T * const force, DOUBLE_T * const efric, DOUBLE_T * const fBCME);

			// -- getters -- //
			std::vector<DOUBLE_T> get_grid(UINT_T d) const;
			std::vector<DOUBLE_T> get_rmin(VOID_T) const;
			std::vector<DOUBLE_T> get_rmax(VOID_T) const;
			std::vector<DOUBLE_T> get_dr(VOID_T) const;
			std::vector<UINT_T> get_Nr(VOID_T) const;
			DOUBLE_T get_rmin(UINT_T d) const;
			DOUBLE_T get_rmax(UINT_T d) const;
			DOUBLE_T get_dr(UINT_T d) const;
			UINT_T get_Nr(UINT_T d) const;
			UINT_T get_Ntot(VOID_T) const;
			UINT_T get_forcelen(VOID_T) const;
			UINT_T get_efriclen(VOID_T) const;
			UINT_T get_fBCMElen(VOID_T) const;
			UINT_T get_feflen(VOID_T) const;
			std::vector<DOUBLE_T> get_force(const std::vector<DOUBLE_T>& r) const;
			std::vector<DOUBLE_T> get_efric(const std::vector<DOUBLE_T>& r) const;
			std::vector<DOUBLE_T> get_fBCME(const std::vector<DOUBLE_T>& r) const;

			// -- conversion between r & index -- //
			UINT_T r_to_index_raw(const std::vector<DOUBLE_T>& r) const;
			UINT_T r_to_index(const std::vector<DOUBLE_T>& r) const;
			std::vector<DOUBLE_T> index_to_r_raw(UINT_T index) const;
			std::vector<DOUBLE_T> index_to_r(UINT_T index) const;
	};
};
#endif
