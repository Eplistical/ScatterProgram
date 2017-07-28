#include <sstream>
#include "scatter_exceptions.hpp"
#include "vector.hpp"
#include "grid_t.hpp"

using namespace scatter;
// constructor
grid_t::grid_t( const std::vector<DOUBLE_T>& rmin, 
				const std::vector<DOUBLE_T>& rmax, 
				const std::vector<DOUBLE_T>& boundary_rmin, 
				const std::vector<DOUBLE_T>& boundary_rmax, 
				const std::vector<UINT_T>& Nr) :
				_rmin(rmin), 
				_rmax(rmax),
				_boundary_rmin(boundary_rmin), 
				_boundary_rmax(boundary_rmax),
			   	_Nr(Nr)	
{
	const UINT_T Ntot = get_Ntot();
	_dr = (_rmax - _rmin) / _Nr;
	_dim = _Nr.size();
	_dim2 = _dim * _dim;
	_forcelen = _dim * Ntot;
	_efriclen = _dim2 * Ntot;
	_fBCMElen = _dim * Ntot;
	_forceoffset = 0;
	_efricoffset = _forcelen;
	_fBCMEoffset = _forcelen + _efriclen;
	_fef_data_ptr = nullptr;
}

// setter
std::vector<DOUBLE_T>& grid_t::get_fef_ref(VOID_T) noexcept
{
	return _fef;
}

VOID_T grid_t::set_fef_data_ptr(DOUBLE_T* ptr) noexcept
{
	_fef_data_ptr = ptr;
}

VOID_T grid_t::alloc_fef_space(VOID_T) noexcept
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

std::vector<DOUBLE_T> grid_t::get_rmin(VOID_T) const noexcept
{
	return _rmin;
}

std::vector<DOUBLE_T> grid_t::get_rmax(VOID_T) const noexcept
{
	return _rmax;
}

std::vector<DOUBLE_T> grid_t::get_dr(VOID_T) const noexcept
{
	return _dr;
}

std::vector<DOUBLE_T> grid_t::get_boundary_rmin(VOID_T) const noexcept
{
	return _boundary_rmin;
}

std::vector<DOUBLE_T> grid_t::get_boundary_rmax(VOID_T) const noexcept
{
	return _boundary_rmax;
}

std::vector<UINT_T> grid_t::get_Nr(VOID_T) const noexcept
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

DOUBLE_T grid_t::get_boundary_rmin(UINT_T d) const
{
	return _boundary_rmin.at(d);
}

DOUBLE_T grid_t::get_boundary_rmax(UINT_T d) const
{
	return _boundary_rmax.at(d);
}

DOUBLE_T grid_t::get_dr(UINT_T d) const
{
	return _dr.at(d);
}

UINT_T grid_t::get_Nr(UINT_T d) const
{
	return _Nr.at(d);
}

UINT_T grid_t::get_Ntot(VOID_T) const noexcept
{
	UINT_T rst = 1;
	for (UINT_T d : _Nr) rst *= d;
	return rst;
}

UINT_T grid_t::get_forcelen(VOID_T) const noexcept
{
	return _forcelen;
}

UINT_T grid_t::get_efriclen(VOID_T) const noexcept
{
	return _efriclen;
}

UINT_T grid_t::get_fBCMElen(VOID_T) const noexcept
{
	return _fBCMElen;
}

UINT_T grid_t::get_feflen(VOID_T) const noexcept
{
	return _forcelen + _efriclen + _fBCMElen;
}

DOUBLE_T* grid_t::set_fef_data_ptr(VOID_T) const noexcept
{
	return _fef_data_ptr;
}

std::vector<DOUBLE_T> grid_t::get_force(const std::vector<DOUBLE_T>& r) const
{
	const UINT_T begin = r_to_index(r) * _dim;
	if (_fef_data_ptr != nullptr) {
		return std::vector<DOUBLE_T>(_fef_data_ptr + begin, _fef_data_ptr + begin + _dim);
	}
	else {
		return std::vector<DOUBLE_T>(_fef.begin() + begin, _fef.begin() + begin + _dim);
	}
}

std::vector<DOUBLE_T> grid_t::get_efric(const std::vector<DOUBLE_T>& r) const
{
	const UINT_T begin = _efricoffset + r_to_index(r) * _dim2;
	if (_fef_data_ptr != nullptr) {
		return std::vector<DOUBLE_T>(_fef_data_ptr + begin, _fef_data_ptr + begin + _dim2);
	}
	else {
		return std::vector<DOUBLE_T>(_fef.begin() + begin, _fef.begin() + begin + _dim2);
	}
}

std::vector<DOUBLE_T> grid_t::get_fBCME(const std::vector<DOUBLE_T>& r) const
{
	const UINT_T begin = _fBCMEoffset + r_to_index(r) * _dim;
	if (_fef_data_ptr != nullptr) {
		return std::vector<DOUBLE_T>(_fef_data_ptr + begin, _fef_data_ptr + begin + _dim);
	}
	else {
		return std::vector<DOUBLE_T>(_fef.begin() + begin, _fef.begin() + begin + _dim);
	}
}

BOOL_T grid_t::is_in_boundary(const std::vector<DOUBLE_T>& r, UINT_T d) const
{
	return r.at(d) > _boundary_rmin.at(d) and r.at(d) < _boundary_rmax.at(d);
}

BOOL_T grid_t::is_in_boundary(const std::vector<DOUBLE_T>& r) const
{
	for (UINT_T d = 0; d < _dim; ++d) 
		if (!is_in_boundary(r, d)) return false;
	return true;
}

BOOL_T grid_t::is_leaving_boundary(const std::vector<DOUBLE_T>& r, const std::vector<DOUBLE_T>& p, UINT_T d) const
{
	if (r.at(d) <= _boundary_rmin.at(d) and p.at(d) <= 0) return true; 
	if (r.at(d) >= _boundary_rmax.at(d) and p.at(d) >= 0) return true; 
	return false;
}

BOOL_T grid_t::is_leaving_boundary(const std::vector<DOUBLE_T>& r, const std::vector<DOUBLE_T>& p) const
{
	for (UINT_T d = 0; d < _dim; ++d) 
		if (is_leaving_boundary(r, p, d)) return true;
	return false;
}

BOOL_T grid_t::is_entering_boundary(const std::vector<DOUBLE_T>& r, const std::vector<DOUBLE_T>& p, UINT_T d) const
{
	if (r.at(d) <= _boundary_rmin.at(d) and p.at(d) >= 0) return true; 
	if (r.at(d) >= _boundary_rmax.at(d) and p.at(d) <= 0) return true; 
	return false;
}

BOOL_T grid_t::is_entering_boundary(const std::vector<DOUBLE_T>& r, const std::vector<DOUBLE_T>& p) const
{
	for (UINT_T d = 0; d < _dim; ++d) 
		if (is_entering_boundary(r, p, d)) return true;
	return false;
}

// -- r_to_index -- //

UINT_T grid_t::r_to_index_raw(const std::vector<DOUBLE_T>& r) const
{
	UINT_T rst = 0;
	INT_T coef = 1;
	INT_T d_index;
	for (UINT_T d = 0; d < _dim; ++d) {
		d_index = static_cast<INT_T>((r.at(d) - _rmin.at(d)) / _dr.at(d) + 0.5);
		// throw if out of range
		if (d_index >= _Nr.at(d) or d_index < 0) {
			std::stringstream errmsg;
			errmsg << "r_to_index_raw: r[" << d << "] = " << r.at(d) << " out of range!\n";
			throw scatter::OutofRangeError(errmsg.str());
		}
		rst += d_index * coef;
		coef *= _Nr.at(d);
	}
	return rst;
}

UINT_T grid_t::r_to_index(const std::vector<DOUBLE_T>& r) const
{
	// wrapper for r_to_index_raw
	// THIS IS FOR 2D SCATTERING MODEL ONLY!
	// modify z to make all z < zmin be treated as zmin
	std::vector<DOUBLE_T> effect_r = r;
	effect_r.at(1) = std::max(_rmin.at(1), effect_r.at(1));
	return r_to_index_raw(effect_r);
}

// -- index_to_r -- //

std::vector<DOUBLE_T> grid_t::index_to_r_raw(UINT_T index) const
{
	UINT_T coef = get_Ntot();
	std::vector<DOUBLE_T> rst(_dim);
	INT_T d_index;
	for (INT_T d = _dim - 1; d >= 0; --d) {
		coef /= _Nr.at(d);
		d_index = static_cast<INT_T>(index / coef + 0.5);
		// throw if out of range
		if (d_index >= _Nr.at(d) or d_index < 0) {
			std::stringstream errmsg;
			errmsg << "index_to_r_raw: dimension " << d << "is detected out of range!\n";
			throw scatter::OutofRangeError(errmsg.str());
		}
		rst.at(d) = _rmin.at(d) + _dr.at(d) * d_index;
		index %= coef; 
	}
	return rst;
}

std::vector<DOUBLE_T> grid_t::index_to_r(UINT_T index) const
{
	// wrapper for index_to_r_raw
	return index_to_r_raw(index);
}
