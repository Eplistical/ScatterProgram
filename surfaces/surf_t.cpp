#include <cstring>
#include <vector>
#include "vector.hpp"
#include "types.hpp"
#include "single_surf.hpp"
#include "rem.hpp"
#include "surf_t.hpp"

using namespace scatter;
using scatter::rem::dim;

// constructor and para initializer
surf_t::surf_t(){}

surf_t::surf_t(size_t Nsurf){
	_Nsurf = Nsurf;
	_gamma = single_surf_t(dim);
	_energy.resize(_Nsurf);
	for(size_t i = 0; i < _Nsurf; i++){
		_energy.at(i) = single_surf_t(dim);
	}
}

// setter
void surf_t::set_gamma(const std::vector<surf_info_t>& info){
	_gamma.init_surfptr(info);
}

void surf_t::set_energy(size_t i, const std::vector<surf_info_t>& info){
	_energy.at(i).init_surfptr(info);
}

void surf_t::set_abs_gamma_threash(double x){

}

// getter
size_t surf_t:: get_surf_number(void) const{
	return _Nsurf;
}

single_surf_t surf_t::get_gamma(void) const{
	return _gamma; 
}

single_surf_t surf_t::get_energy(int i) const{
	return _energy.at(i);
}

std::vector<double> surf_t::get_gamma_para(int d) const{ 
	return _gamma.get_para_as_vec(d); 
}

std::vector<double> surf_t::get_energy_para(int i, int d) const{ 
	return _energy.at(i).get_para_as_vec(d); 
}

// U
std::vector<double> surf_t::vU(int i, const std::vector<double>& r) const{ 
	return _energy.at(i).get_U(r); 
}

std::vector<double> surf_t::vU0(const std::vector<double>& r) const{ 
	return vU(0, r); 
}

std::vector<double> surf_t::vU1(const std::vector<double>& r) const{
	return vU(1, r); 
}

double surf_t::fU(int i, const std::vector<double>& r) const{
	return sum(vU(i, r)); 
}

double surf_t::fU0(const std::vector<double>& r) const {
	return fU(0, r); 
}

double surf_t::fU1(const std::vector<double>& r) const {
	return fU(1, r); 
}

// F
std::vector<double> surf_t::vdUdr(int i, const std::vector<double>& r) const{ 
	return _energy.at(i).get_dUdx(r); 
}

std::vector<double> surf_t::vF(int i, const std::vector<double>& r) const{ 
	return -1.0 * vdUdr(i, r);
}

std::vector<double> surf_t::fF0(const std::vector<double>& r) const{ 
	return vF(0, r); 
}

std::vector<double> surf_t::fF1(const std::vector<double>& r) const{ 
	return vF(1, r); 
}

// Gamma
std::vector<double> surf_t::vGamma(const std::vector<double>& r) const{ 
	return _gamma.get_U(r);  
}

double surf_t::fGamma(const std::vector<double>& r, double Gamma0) const{
	return product(vGamma(r)) * Gamma0;
}

std::vector<double> surf_t::vGammader(const std::vector<double>& r) const{
	return _gamma.get_dUdx(r); 
}

std::vector<double> surf_t::fGammader(const std::vector<double>& r, double Gamma0) const{
	const std::vector<double> tmp_gamma = vGamma(r);
	const std::vector<double> tmp_gammader = vGammader(r);
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

bool surf_t::small_gamma(const std::vector<double>& r, double Gamma0) const{
	return (fGamma(r, Gamma0) < _abs_gamma_threash); 
}
