#ifndef _SCATTER_GRID_T_HPP
#define _SCATTER_GRID_T_HPP
// module for grid_t

#include <vector>

namespace scatter{
	class grid_t{
		private:
			// data 
			std::vector<double> _rmin;
			std::vector<double> _rmax;
			std::vector<double> _dr;
			std::vector<size_t> _Nr;
			std::vector<double> _fef;
			size_t _forcelen;
			size_t _efriclen;
			size_t _fBCMElen;
			size_t _forceoffset;
			size_t _efricoffset;
			size_t _fBCMEoffset;

			// program-dependent 
			size_t r_to_index_filtered(const std::vector<double>& r) const;
		public:
			// constructor
			grid_t() = default;
			~grid_t() = default;
			grid_t(const grid_t&) = default;
			grid_t& operator=(const grid_t&) = default;
			explicit grid_t(const std::vector<double>& rmin, const std::vector<double>& rmax, const std::vector<size_t>& Nr);
			// setter
			std::vector<double>& get_fef_ref(void);
			void alloc_fef_space(void);
			void calc_fef(size_t S0, size_t S1, size_t index);
			// getter
			std::vector<double> get_grid(size_t d) const;
			std::vector<double> get_rmin(void) const;
			std::vector<double> get_rmax(void) const;
			std::vector<double> get_dr(void) const;
			std::vector<size_t> get_Nr(void) const;
			double get_rmin(size_t d) const;
			double get_rmax(size_t d) const;
			double get_dr(size_t d) const;
			size_t get_Nr(size_t d) const;
			size_t get_Ntot(void) const;
			size_t get_forcelen(void) const;
			size_t get_efriclen(void) const;
			size_t get_fBCMElen(void) const;
			size_t get_feflen(void) const;
			std::vector<double> get_force(const std::vector<double>& r) const;
			std::vector<double> get_efric(const std::vector<double>& r) const;
			std::vector<double> get_fBCME(const std::vector<double>& r) const;
			size_t r_to_index(const std::vector<double>& r) const;
			std::vector<double> index_to_r(size_t) const;
	};
};
#endif
