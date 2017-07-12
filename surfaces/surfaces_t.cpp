#include "types.hpp"
#include <cstring>
#include <vector>
#include "vector.hpp"
#include "rem.hpp"
#include "surfaces_collection.hpp"
#include "surfaces_t.hpp"

using namespace scatter;

// constructor and para initializer
surfaces_t::surfaces_t(UINT_T Nsurf) 
{
	_Nsurf = Nsurf;
	_gamma = single_surf_t();
	_energy.resize(_Nsurf);
	for(UINT_T i = 0; i < _Nsurf; ++i){
		_energy.at(i) = single_surf_t();
	}
}

// setter
VOID_T surfaces_t::set_gamma(const std::vector<enumspace::surfmode_enum>& modes, 
							const std::vector<std::vector<DOUBLE_T> >& paras)
{
	_gamma.clear();
	for(UINT_T d = 0; d < rem::dim; ++d){
		_gamma.append_dim(modes.at(d), paras.at(d));
	}
}

VOID_T surfaces_t::set_energy(const std::vector<enumspace::surfmode_enum>& modes, 
								const std::vector<std::vector<DOUBLE_T> >& paras)
{
	UINT_T index = 0;
	for(UINT_T i = 0; i < _Nsurf; ++i) {
		_energy.at(i).clear();
		for(UINT_T d = 0; d < rem::dim; ++d) {
			index = i * rem::dim + d;
			_energy.at(i).append_dim(modes.at(index), paras.at(index));
		}
	}
}

VOID_T surfaces_t::set_abs_gamma_threash(DOUBLE_T x)
{
	_abs_gamma_threash = x; 
}

// getter
UINT_T surfaces_t::get_surf_number(VOID_T) const
{
	return _Nsurf; 
}

single_surf_t surfaces_t::get_gamma(VOID_T) const
{
	return _gamma; 
}

single_surf_t surfaces_t::get_energy(INT_T i) const
{
	return _energy.at(i);
}

std::vector<DOUBLE_T> surfaces_t::get_gamma_para(INT_T d) const
{
	 return _gamma.get_para(d);
}

std::vector<DOUBLE_T> surfaces_t::get_energy_para(INT_T i, INT_T d) const
{
	return _energy.at(i).get_para(d);
}

std::vector<std::vector<DOUBLE_T> > surfaces_t::get_gamma_para_blank(VOID_T) const
{
	std::vector<std::vector<DOUBLE_T> > rst(1);
	UINT_T N = 0;
	for (UINT_T d = 0; d < rem::dim; ++d) {
		N += _gamma.get_Npara(d);
	}
	rst[0].resize(N);
	return rst;
}

std::vector<std::vector<DOUBLE_T> > surfaces_t::get_energy_para_blank(VOID_T) const
{
	std::vector<std::vector<DOUBLE_T> > rst(_Nsurf);
	UINT_T N = 0;
	for(UINT_T i = 0; i < _Nsurf; ++i) {
		N = 0;
		for (UINT_T d = 0; d < rem::dim; ++d) {
			N += _energy.at(i).get_Npara(d);
		}
		rst[i].resize(N);
	}
	return rst;
}


// U
std::vector<DOUBLE_T> surfaces_t::vU(INT_T i, const std::vector<DOUBLE_T>& r) const
{
	return _energy.at(i).get_U(r);
}

DOUBLE_T surfaces_t::fU(INT_T i, INT_T d, DOUBLE_T x) const
{
	return _energy.at(i).get_U(d, x);
}

DOUBLE_T surfaces_t::fU(INT_T i, const std::vector<DOUBLE_T>& r) const
{
	return sum(vU(i, r));
}

// F
std::vector<DOUBLE_T> surfaces_t::vdUdr(INT_T i, const std::vector<DOUBLE_T>& r) const
{
	return _energy.at(i).get_dUdx(r);
}

std::vector<DOUBLE_T> surfaces_t::fF(INT_T i, const std::vector<DOUBLE_T>& r) const
{
	return -1.0 * vdUdr(i, r);
}

DOUBLE_T surfaces_t::fF(INT_T i, INT_T d, DOUBLE_T x) const
{
	return -1.0 * _energy.at(i).get_dUdx(d, x);
}

// Gamma
std::vector<DOUBLE_T> surfaces_t::vGamma(const std::vector<DOUBLE_T>& r) const
{
	return _gamma.get_U(r);
}

DOUBLE_T surfaces_t::fGamma(const std::vector<DOUBLE_T>& r, DOUBLE_T Gamma0) const
{
	return product(vGamma(r)) * Gamma0;
}

DOUBLE_T surfaces_t::fGamma(INT_T d, DOUBLE_T x) const
{
	return _gamma.get_U(d, x);
}

std::vector<DOUBLE_T> surfaces_t::vGammader(const std::vector<DOUBLE_T>& r) const
{
	return _gamma.get_dUdx(r);
}

std::vector<DOUBLE_T> surfaces_t::fGammader(const std::vector<DOUBLE_T>& r, DOUBLE_T Gamma0) const
{
	const std::vector<DOUBLE_T> tmp_gamma = vGamma(r);
	const std::vector<DOUBLE_T> tmp_gammader = vGammader(r);
	const UINT_T dim = scatter::rem::dim;
	std::vector<DOUBLE_T> rst(dim);
	// tmp = (g1,g2,..)
	std::vector<DOUBLE_T> tmp = tmp_gamma;
	for(UINT_T d = 0; d < dim; ++d) {
		tmp.at(d) = tmp_gammader.at(d);
		rst.at(d) = product(tmp) * Gamma0;
		tmp.at(d) = tmp_gamma.at(d);
	}
	return rst;
}

DOUBLE_T surfaces_t::fGammader(INT_T d, DOUBLE_T x) const
{
	return _gamma.get_dUdx(d, x);
}

BOOL_T surfaces_t::small_gamma(const std::vector<DOUBLE_T>& r, DOUBLE_T Gamma0) const
{
	return (fGamma(r, Gamma0) < _abs_gamma_threash);
}
