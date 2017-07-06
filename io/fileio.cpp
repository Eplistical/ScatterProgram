#include <string>
#include <vector>
#include <stdexcept>
#include "ioer.hpp"
#include "io_var.hpp"
#include "rem.hpp"
#include "fileio.hpp"
#include "simulation.hpp"
#include "run.hpp"

using namespace scatter;
using ioer::output_t;
using ioer::input_t;

void rwinit(char op)
{
	using scatter::rem::dim;
	using scatter::simulation::mass;
	using scatter::simulation::inittemp;
	using scatter::simulation::Ntraj;

	using scatter::simulation::r0p0;
	if (op == 'w') {
		output_t dest(scatter::io::initfile, ios::out | ios::binary);
		dest.write(dim, mass, inittemp);
		dest.write(r0p0);
	}
	if (op == 'r') {
		input_t source(scatter::io::initfile, ios::in | ios::binary);
		mass.resize(dim);
		r0p0.resize(2 * dim * Ntraj);
		source.read(dim, mass, inittemp);
		source.read(r0p0);
	}
}

void rwdat(char op){
	using scatter::surfaces_obj;
	using scatter::grid_obj;
	using scatter::surfaces::surfnum;
	using scatter::simulation::mass;
	using scatter::rem::dim;
	using scatter::rem::kT;
	using scatter::rem::Gamma0;

	if (op == 'w') {
		output_t dest(scatter::io::datfile, ios::out | ios::binary);
		// -- header part -- //
		dest.write(dim, surfnum, kT, Gamma0, mass);
		// surfaces para info
		for(size_t d = 0; d < dim; ++d){
			dest.write(surfaces_obj.get_gamma_para(d));
		}
		for(size_t i = 0; i < surfnum; i++){
			for(size_t d = 0; d < dim; d++){
				dest.write(surfaces_obj.get_energy_para(i, d));
			}
		}
		// grid info
		dest.write(grid_obj.get_rmin(), grid_obj.get_rmax(), grid_obj.get_dr());
		// -- data part -- //
		dest.write(grid_obj.get_fef_ref());
	}
	else if (op == 'r') {
		//// NOT FINISHED HERE
		input_t source(scatter::io::datfile, ios::in | ios::binary);
		// -- header part -- //
		mass.resize(dim);
		source.read(dim, surfnum, kT, Gamma0, mass);
		// surfaces para info
		for(size_t d = 0; d < dim; ++d){
			//source.read(surfaces_obj.get_gamma_para(d));
		}
		for(size_t i = 0; i < surfnum; i++){
			for(size_t d = 0; d < dim; d++){
				//source.read(surfaces_obj.get_energy_para(i, d));
			}
		}
		// grid info
		//source.read(grid_obj.get_rmin(), grid_obj.get_rmax(), grid_obj.get_dr());
		// -- data part -- //
		//source.read(grid_obj.get_fef_ref());
	}
}

// API
void scatter::io::savedat(void){
    ioer::info_nonewline("saving data to " + scatter::io::datfile + " ...  ");
    rwdat('w');
    ioer::info_nonewline("done");
}

void scatter::io::loaddat(void){
    ioer::info_nonewline("loading data from " + scatter::io::datfile + " ...  ");
    rwdat('r');
    ioer::info("done");
}

void scatter::io::saveinit(void){
    ioer::info_nonewline("saving r0p0 to " + scatter::io::initfile + " ...  ");
    rwinit('w');
    ioer::info("done");
}

void scatter::io::loadinit(void){
    ioer::info_nonewline("loading r0p0 to " + scatter::io::initfile + " ...  ");
    rwinit('r');
    ioer::info("done");
}
