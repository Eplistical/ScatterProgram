#include <cstring>
#include <vector>
#include "vector.hpp"
#include "rem.hpp"
#include "surfaces_collection.hpp"
#include "surfaces_t.hpp"

using namespace scatter;

// constructor and para initializer
surfaces_t::surfaces_t(size_t Nsurf){
	_Nsurf = Nsurf;
	_gamma = single_surf_t();
	_energy.resize(_Nsurf);
	for(size_t i = 0; i < _Nsurf; ++i){
		_energy.at(i) = single_surf_t();
	}
}

// setter
void surfaces_t::set_gamma(const std::vector<enumspace::surfmode_enum>& modes, const std::vector<std::vector<double>>& paras){
	_gamma.clear();
	for(size_t d = 0, N = modes.size(); d < N; ++d){
		_gamma.append_dim(modes.at(d), paras.at(d));
	}
}

void surfaces_t::set_energy(size_t i, const std::vector<enumspace::surfmode_enum>& modes, const std::vector<std::vector<double>>& paras){
	_energy.at(i).clear();
	for(size_t d = 0, N = modes.size(); d < N; ++d){
		_energy.at(i).append_dim(modes.at(d), paras.at(d));
	}
}

void surfaces_t::set_abs_gamma_threash(double x)
{ _abs_gamma_threash = x; }

// getter
size_t surfaces_t:: get_surf_number(void) const
{ return _Nsurf; }
single_surf_t surfaces_t::get_gamma(void) const
{ return _gamma; }
single_surf_t surfaces_t::get_energy(int i) const
{ return _energy.at(i); }
std::vector<double> surfaces_t::get_gamma_para(int d) const
{  return _gamma.get_para(d); }
std::vector<double> surfaces_t::get_energy_para(int i, int d) const
{ return _energy.at(i).get_para(d); }
// U
std::vector<double> surfaces_t::vU(int i, const std::vector<double>& r) const
{ return _energy.at(i).get_U(r); }
double surfaces_t::fU(int i, int d, double x) const
{ return _energy.at(i).get_U(d, x); }
double surfaces_t::fU(int i, const std::vector<double>& r) const
{ return sum(vU(i, r)); }
// F
std::vector<double> surfaces_t::vdUdr(int i, const std::vector<double>& r) const
{ return _energy.at(i).get_dUdx(r); }
std::vector<double> surfaces_t::fF(int i, const std::vector<double>& r) const
{ return -1.0 * vdUdr(i, r); }
double surfaces_t::fF(int i, int d, double x) const
{ return _energy.at(i).get_dUdx(d, x); }
// Gamma
std::vector<double> surfaces_t::vGamma(const std::vector<double>& r) const
{ return _gamma.get_U(r); }
double surfaces_t::fGamma(const std::vector<double>& r, double Gamma0) const
{ return product(vGamma(r)) * Gamma0; }
std::vector<double> surfaces_t::vGammader(const std::vector<double>& r) const
{ return _gamma.get_dUdx(r); }
std::vector<double> surfaces_t::fGammader(const std::vector<double>& r, double Gamma0) const
{
	const std::vector<double> tmp_gamma = vGamma(r);
	const std::vector<double> tmp_gammader = vGammader(r);
	const size_t dim = scatter::rem::dim;
	std::vector<double> rst(dim);
	// tmp = (g1,g2,..)
	std::vector<double> tmp = tmp_gamma;
	for(size_t d = 0;d < dim;d++){
		tmp.at(d) = tmp_gammader.at(d);
		rst.at(d) = product(tmp) * Gamma0;
		tmp.at(d) = tmp_gamma.at(d);
	}
	return rst;
}

bool surfaces_t::small_gamma(const std::vector<double>& r, double Gamma0) const
{ return (fGamma(r, Gamma0) < _abs_gamma_threash); }
