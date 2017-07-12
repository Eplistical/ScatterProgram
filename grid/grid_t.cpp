#include "types.hpp"
#ifdef _DEBUG
#include "debugtools.hpp"
#endif

#include <sstream>
#include "scatter_exceptions.hpp"
#include "vector.hpp"
#include "rem.hpp"
#include "grid_t.hpp"

using namespace scatter;
using scatter::rem::dim;
using scatter::rem::dim2;
// constructor
grid_t::grid_t( const std::vector<DOUBLE_T>& rmin, 
				const std::vector<DOUBLE_T>& rmax, 
				const std::vector<UINT_T>& Nr) :
				_rmin(rmin), 
				_rmax(rmax),
			   	_Nr(Nr)	
{
	const UINT_T Ntot = get_Ntot();
	_dr = (_rmax - _rmin) / _Nr;
	_forcelen = dim * Ntot;
	_efriclen = dim2 * Ntot;
	_fBCMElen = dim * Ntot;
	_forceoffset = 0;
	_efricoffset = _forcelen;
	_fBCMEoffset = _forcelen + _efriclen;
}

// setter
std::vector<DOUBLE_T>& grid_t::get_fef_ref(void) 
{
	return _fef;
}

void grid_t::alloc_fef_space(void)
{
	_fef.assign(get_feflen(), 0.0);
}

// getter
std::vector<DOUBLE_T> grid_t::get_grid(UINT_T d) const
{
	std::vector<DOUBLE_T> rst(_Nr.at(d));
	for(UINT_T i = 0, N = _Nr.at(d); i < N; ++i){
		rst[i] = _rmin[d] + i * _dr[d];
	}
	return rst;
}

std::vector<DOUBLE_T> grid_t::get_rmin(void) const
{
	return _rmin;
}

std::vector<DOUBLE_T> grid_t::get_rmax(void) const
{
	return _rmax;
}

std::vector<DOUBLE_T> grid_t::get_dr(void) const
{
	return _dr;
}

std::vector<UINT_T> grid_t::get_Nr(void) const
{
	return _Nr;
}

DOUBLE_T grid_t::get_rmin(UINT_T d) const
{
	return _rmin.at(d);
}

DOUBLE_T grid_t::get_rmax(UINT_T d) const
{
	return _rmax.at(d);
}

DOUBLE_T grid_t::get_dr(UINT_T d) const
{
	return _dr.at(d);
}

UINT_T grid_t::get_Nr(UINT_T d) const
{
	return _Nr.at(d);
}

UINT_T grid_t::get_Ntot(void) const
{
	UINT_T rst = 1;
	for (UINT_T d : _Nr) rst *= d;
	return rst;
}

UINT_T grid_t::get_forcelen(void) const
{
	return _forcelen;
}

UINT_T grid_t::get_efriclen(void) const
{
	return _efriclen;
}

UINT_T grid_t::get_fBCMElen(void) const
{
	return _fBCMElen;
}

UINT_T grid_t::get_feflen(void) const
{
	return _forcelen + _efriclen + _fBCMElen;
}

std::vector<DOUBLE_T> grid_t::get_force(const std::vector<DOUBLE_T>& r) const
{
	return subvec(_fef, r_to_index_filtered(r) * dim, dim);
}

std::vector<DOUBLE_T> grid_t::get_efric(const std::vector<DOUBLE_T>& r) const
{
	return subvec(_fef, _efricoffset + r_to_index_filtered(r) * dim2, dim2);
}

std::vector<DOUBLE_T> grid_t::get_fBCME(const std::vector<DOUBLE_T>& r) const
{
	return subvec(_fef, _fBCMEoffset + r_to_index_filtered(r) * dim, dim);
}

UINT_T grid_t::r_to_index(const std::vector<DOUBLE_T>& r) const
{
	UINT_T rst = 0;
	INT_T coef = 1;
	INT_T d_index;
	for (UINT_T d = 0; d < rem::dim; ++d) {
		d_index = static_cast<INT_T>((r.at(d) - _rmin.at(d)) / _dr.at(d) + 0.5);
		// throw if out of range
		if (d_index >= _Nr.at(d) or d_index < 0) {
			std::stringstream errmsg;
			errmsg << "r_to_index: r[" << d << "] = " << r.at(d) << " out of range!\n";
			throw scatter::OutofRangeError(errmsg.str());
		}
		rst += d_index * coef;
		coef *= _Nr.at(d);
	}
	return rst;
}

std::vector<DOUBLE_T> grid_t::index_to_r(UINT_T index) const
{
	UINT_T coef = get_Ntot();
	std::vector<DOUBLE_T> rst(rem::dim);
	INT_T d_index;
	for (INT_T d = rem::dim - 1; d >= 0; --d) {
		coef /= _Nr.at(d);
		d_index = static_cast<INT_T>(index / coef + 0.5);
		// throw if out of range
		if (d_index >= _Nr.at(d) or d_index < 0) {
			std::stringstream errmsg;
			errmsg << "index_to_r: dimension " << d << "is detected out of range!\n";
			throw scatter::OutofRangeError(errmsg.str());
		}
		rst.at(d) = _rmin.at(d) + _dr.at(d) * d_index;
		index %= coef; 
	}
	return rst;
}

UINT_T grid_t::r_to_index_filtered(const std::vector<DOUBLE_T>& r) const
{
	// THIS IS FOR 2D SCATTERING MODEL ONLY!
	// modify z to make all z < zmin be treated as zmin
	std::vector<DOUBLE_T> effect_r = r;
	effect_r.at(1) = std::max(_rmin.at(1), effect_r.at(1));
	return r_to_index(effect_r);
}
