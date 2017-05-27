#include <random>
#include "vector.hpp"
#include "randomer.hpp"
#include "simulation_var.hpp"
#include "initmode.hpp"
#include "initstate_t.hpp"

using namespace scatter;
using namespace scatter::rem;
using namespace scatter::simulation;

// constructor
initstate_t::initstate_t(void){}
initstate_t::initstate_t(size_t vibstate, size_t elestate, double temp) :
	_temp(temp),
	_vibstate(vibstate), _elestate(elestate), 
	_mode(dim), _ravg(dim), _pavg(dim){}

// getter & setter
std::vector<double>& initstate_t::get_rp_ref(void){
	return _rp;
}

// setter
void initstate_t::set_mode(const std::vector<scatter::mode::initmode>& mode){
	_mode = mode; 
}

void initstate_t::set_mode(size_t d, const scatter::mode::initmode& mode){
	_mode.at(d) = mode; 
}

void initstate_t::set_ravg(const std::vector<double>& ravg){
	_ravg = ravg;
}

void initstate_t::set_ravg(size_t d, double ravg){
	_ravg.at(d) = ravg;
}

void initstate_t::set_pavg(const std::vector<double>& pavg){
	_pavg = pavg;
}

void initstate_t::set_pavg(size_t d, double pavg){
	_pavg.at(d) = pavg;
}

void initstate_t::generate_rp(void){
	// get simulation properties
	const std::vector<double> sigmar = pow(_temp / omega.at(_elestate) / omega.at(_elestate) / mass, 0.5);
	const std::vector<double> sigmap = pow(_temp * mass, 0.5);
	// prepare r0p0
	std::vector<double> r, p;
	_rp.clear();
	double tmp;
	// loop over Ntraj
	for(size_t traj = 0; traj < Ntraj; traj++){
		r.clear();
		p.clear();
		// loop over dim
		for(size_t d = 0; d < dim; d++){
			if(_mode.at(d) == mode::initmode::DELTA){
				r.push_back(_ravg.at(d));
				p.push_back(_pavg.at(d));
			}
			else if(_mode.at(d) == mode::initmode::RING){
				tmp = randomer::rand(0, 2 * M_PI);
				r.push_back(sqrt(2 * _vibstate + 1.0) * cos(tmp) * sqrt(1.0 / mass.at(0) / omega.at(_elestate) / hbar));
				p.push_back(sqrt(2 * _vibstate + 1.0) * sin(tmp) * sqrt(mass.at(0) * omega.at(_elestate) * hbar));
			}
			else if(_mode.at(d) == mode::initmode::RP_GAUSSIAN){
				r.push_back(randomer::normrand(_ravg.at(d), sigmar.at(d)));
				p.push_back(randomer::normrand(_pavg.at(d), sigmap.at(d)));
			}
			else if(_mode.at(d) == mode::initmode::P_GAUSSIAN){
				r.push_back(_ravg.at(d));
				p.push_back(randomer::normrand(_pavg.at(d), sigmap.at(d)));
			}
		}
		// insert r, p for traj^th particle to _rp
		_rp.insert(_rp.end(), r.begin(), r.end());
		_rp.insert(_rp.end(), p.begin(), p.end());
	}
}

// getter
size_t initstate_t::get_vibstate(void) const{
	return _vibstate;
}

size_t initstate_t::get_elestate(void) const{
	return _elestate;
}

double initstate_t::get_temp(void) const{
	return _temp;
}

mode::initmode initstate_t::get_mode(size_t d) const{
	return _mode.at(d);
}

double initstate_t::get_ravg(size_t d) const{
	return _ravg.at(d);
}

double initstate_t::get_pavg(size_t d) const{
	return _pavg.at(d);
}
