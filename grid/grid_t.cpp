#include <sstream>
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

// getter & setter
std::vector<double>& grid_t::get_fef_ref(void) 
{ return _fef; }
// getter
std::vector<double> grid_t::get_grid(size_t d) const{
	std::vector<double> rst(_Nr.at(d));
	for(size_t i = 0, N = _Nr.at(d); i < N; ++i){
		rst[d] = _rmin[d] + i * _dr[d];
	}
	return rst;
}
std::vector<double> grid_t::get_rmin(void) const
{ return _rmin; }
std::vector<double> grid_t::get_rmax(void) const
{ return _rmax; }
std::vector<double> grid_t::get_dr(void) const
{ return _dr; }
std::vector<size_t> grid_t::get_Nr(void) const
{ return _Nr; }
double grid_t::get_rmin(size_t i) const
{ return _rmin.at(i); }
double grid_t::get_rmax(size_t i) const
{ return _rmax.at(i); }
double grid_t::get_dr(size_t i) const
{ return _dr.at(i); }
size_t grid_t::get_Nr(size_t i) const
{ return _Nr.at(i); }
size_t grid_t::get_Ntot(void) const
{ return product(_Nr); }
size_t grid_t::get_forcelen(void) const
{ return _forcelen; }
size_t grid_t::get_efriclen(void) const
{ return _efriclen; }
size_t grid_t::get_fBCMElen(void) const
{ return _fBCMElen; }
std::vector<double> grid_t::get_force(const std::vector<double>& r) const
{ return subvec(_fef, r_to_index(r) * dim, dim); }
std::vector<double> grid_t::get_efric(const std::vector<double>& r) const
{ return subvec(_fef, _efricoffset + r_to_index(r) * dim2, dim2); }
std::vector<double> grid_t::get_fBCME(const std::vector<double>& r) const
{ return subvec(_fef, _fBCMEoffset + r_to_index(r) * dim, dim); }

// helper
size_t grid_t::r_to_index(const std::vector<double>& r) const{	
	if(r.at(0) < _rmin.at(0) or r.at(0) > _rmax.at(0)){
		std::stringstream errmsg;
		errmsg 
			<< "r_to_index: x out of range! -- x = " << r.at(0) 
			<< std::endl;
		throw std::out_of_range(errmsg.str());
	}
	const double x = r.at(0);
	const double z = std::max(_rmin.at(1), std::min(r.at(1), _rmax.at(1) - _dr.at(1)));
	const size_t jx = static_cast<size_t>( (x - _rmin.at(0)) / _dr.at(0) );
	const size_t jz = static_cast<size_t>( (z - _rmin.at(1)) / _dr.at(1) );
	const size_t index = jz * _Nr.at(0) + jx;
	return index;
}
