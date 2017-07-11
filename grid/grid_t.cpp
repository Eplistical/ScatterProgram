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
grid_t::grid_t( const std::vector<double>& rmin, 
				const std::vector<double>& rmax, 
				const std::vector<size_t>& Nr) :
				_rmin(rmin), 
				_rmax(rmax),
			   	_Nr(Nr)	
{
	const size_t Ntot = get_Ntot();
	_dr = (_rmax - _rmin) / _Nr;
	_forcelen = dim * Ntot;
	_efriclen = dim2 * Ntot;
	_fBCMElen = dim * Ntot;
	_forceoffset = 0;
	_efricoffset = _forcelen;
	_fBCMEoffset = _forcelen + _efriclen;
}

// setter
std::vector<double>& grid_t::get_fef_ref(void) 
{
	return _fef;
}

void grid_t::alloc_fef_space(void)
{
	_fef.assign(get_feflen(), 0.0);
}

// getter
std::vector<double> grid_t::get_grid(size_t d) const
{
	std::vector<double> rst(_Nr.at(d));
	for(size_t i = 0, N = _Nr.at(d); i < N; ++i){
		rst[i] = _rmin[d] + i * _dr[d];
	}
	return rst;
}

std::vector<double> grid_t::get_rmin(void) const
{
	return _rmin;
}

std::vector<double> grid_t::get_rmax(void) const
{
	return _rmax;
}

std::vector<double> grid_t::get_dr(void) const
{
	return _dr;
}

std::vector<size_t> grid_t::get_Nr(void) const
{
	return _Nr;
}

double grid_t::get_rmin(size_t d) const
{
	return _rmin.at(d);
}

double grid_t::get_rmax(size_t d) const
{
	return _rmax.at(d);
}

double grid_t::get_dr(size_t d) const
{
	return _dr.at(d);
}

size_t grid_t::get_Nr(size_t d) const
{
	return _Nr.at(d);
}

size_t grid_t::get_Ntot(void) const
{
	return product(_Nr);
}

size_t grid_t::get_forcelen(void) const
{
	return _forcelen;
}

size_t grid_t::get_efriclen(void) const
{
	return _efriclen;
}

size_t grid_t::get_fBCMElen(void) const
{
	return _fBCMElen;
}

size_t grid_t::get_feflen(void) const
{
	return _forcelen + _efriclen + _fBCMElen;
}

std::vector<double> grid_t::get_force(const std::vector<double>& r) const
{
	return subvec(_fef, r_to_index_filtered(r) * dim, dim);
}

std::vector<double> grid_t::get_efric(const std::vector<double>& r) const
{
	return subvec(_fef, _efricoffset + r_to_index_filtered(r) * dim2, dim2);
}

std::vector<double> grid_t::get_fBCME(const std::vector<double>& r) const
{
	return subvec(_fef, _fBCMEoffset + r_to_index_filtered(r) * dim, dim);
}

size_t grid_t::r_to_index(const std::vector<double>& r) const
{
	size_t rst = 0;
	int coef = 1;
	int d_index;
	for (size_t d = 0; d < rem::dim; ++d) {
		d_index = static_cast<int>((r.at(d) - _rmin.at(d)) / _dr.at(d) + 0.5);
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

std::vector<double> grid_t::index_to_r(size_t k) const
{
	size_t coef = get_Ntot();
	std::vector<double> rst(rem::dim);
	int d_index;
	for (int d = rem::dim - 1; d >= 0; --d) {
		coef /= _Nr.at(d);
		d_index = static_cast<int>(k / coef + 0.5);
		// throw if out of range
		if (d_index >= _Nr.at(d) or d_index < 0) {
			std::stringstream errmsg;
			errmsg << "index_to_r: dimension " << d << "is detected out of range!\n";
			throw scatter::OutofRangeError(errmsg.str());
		}
		rst.at(d) = _rmin.at(d) + _dr.at(d) * d_index;
		k %= coef; 
	}
	return rst;
}

size_t grid_t::r_to_index_filtered(const std::vector<double>& r) const
{
	// THIS IS FOR 2D SCATTERING MODEL ONLY!
	// modify z to make all z < zmin be treated as zmin
	std::vector<double> effect_r = r;
	effect_r.at(1) = std::max(_rmin.at(1), effect_r.at(1));
	return r_to_index(effect_r);
}
