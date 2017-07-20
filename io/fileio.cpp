#include "types.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>
#include "ioer.hpp"
#include "io_var.hpp"
#include "rem.hpp"
#include "fileio.hpp"
#include "simulation.hpp"
#include "run.hpp"

using scatter::io::out_handler;

using namespace scatter;
using ioer::output_t;
using ioer::input_t;

VOID_T rwinit(CHAR_T op)
{
	using scatter::rem::dim;
	using scatter::simulation::mass;
	using scatter::simulation::inittemp;
	using scatter::simulation::Ntraj;

	using scatter::simulation::r0p0;
	if (op == 'w') {
		output_t dest(io::outdir + io::initfile, std::ios::out | std::ios::binary);
		dest.write(dim, mass, inittemp);
		dest.write(r0p0);
		dest.close();
	}
	if (op == 'r') {
		input_t source(io::indir + io::initfile, std::ios::in | std::ios::binary);
		// -- check header, no assignment -- //
		auto mass_read = mass;
		auto dim_read = dim;
		auto inittemp_read = inittemp;
		source.read(dim_read, mass_read, inittemp_read);

		assert(dim == dim_read);
		assert(mass == mass_read);
		assert(inittemp == inittemp_read);

		// -- read data part -- //
		r0p0.resize(2 * dim * Ntraj);
		source.read(r0p0);
		source.close();
	}
}

VOID_T rwdat(CHAR_T op){
	/**
	 * datfile structure:
	 * 	 dim (uint64)
	 */
	using scatter::surfaces_obj;
	using scatter::grid_obj;
	using scatter::surfaces::surfnum;
	using scatter::surfaces::surfpara;
	using scatter::surfaces::gammapara;
	using scatter::simulation::mass;
	using scatter::rem::dim;
	using scatter::rem::kT;
	using scatter::rem::Gamma0;

	if (op == 'w') {
		output_t dest(io::outdir + io::datfile, std::ios::out | std::ios::binary);
		// -- header part -- //
		dest.write(dim, surfnum, kT, Gamma0, mass);
		// surfaces para info
		for(UINT_T d = 0; d < dim; ++d){
			dest.write(surfaces_obj.get_gamma_para(d));
		}
		for(UINT_T i = 0; i < surfnum; i++){
			for(UINT_T d = 0; d < dim; d++){
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
		input_t source(io::indir + io::datfile, std::ios::in | std::ios::binary);

		// -- check header part, no assignment -- //
		auto dim_read = dim;
		auto surfnum_read = surfnum;
		auto kT_read = kT;
		auto Gamma0_read = Gamma0;
		auto mass_read = mass;
		source.read(dim_read, surfnum_read, kT_read, Gamma0_read, mass_read);

		assert(dim == dim_read);
		assert(surfnum == surfnum_read);
		assert(kT == kT_read);
		assert(Gamma0 == Gamma0_read);
		assert(mass == mass_read);

		// surfaces & gamma para info 
		auto gammapara_read = gammapara;
		auto surfpara_read = surfpara;
		for (UINT_T i = 0; i < dim; ++i) {
			source.read(gammapara_read[i]);
			assert(gammapara[i] == gammapara_read[i]);
		}
		for (UINT_T i = 0; i < surfnum * dim; ++i) {
			source.read(surfpara_read[i]);
			assert(surfpara[i] == surfpara_read[i]);
		}

		// grid info
		auto rmin_read = grid_obj.get_rmin();
		auto rmax_read = grid_obj.get_rmax();
		auto dr_read = grid_obj.get_dr();
		source.read(rmin_read, rmax_read, dr_read);

		assert(grid_obj.get_rmin() == rmin_read);
		assert(grid_obj.get_rmax() == rmax_read);
		assert(grid_obj.get_dr() == dr_read);

		// -- data part, for assignment -- //
		std::vector<DOUBLE_T>& fef = grid_obj.get_fef_ref();
		fef.resize(grid_obj.get_feflen());
		source.read(fef);
		source.close();
	}
}

// API
VOID_T scatter::io::savedat(VOID_T){
    out_handler.info_nonewline("saving data to " + io::outdir + io::datfile + " ...  ");
    rwdat('w');
    out_handler.info("done");
}

VOID_T scatter::io::loaddat(VOID_T){
    out_handler.info_nonewline("loading data from " + io::indir + io::datfile + " ...  ");
    rwdat('r');
    out_handler.info("done");
}

VOID_T scatter::io::saveinit(VOID_T){
    out_handler.info_nonewline("saving r0p0 to " + io::outdir + io::initfile + " ...  ");
    rwinit('w');
    out_handler.info("done");
}

VOID_T scatter::io::loadinit(VOID_T){
    out_handler.info_nonewline("loading r0p0 from " + io::indir + io::initfile + " ...  ");
    rwinit('r');
    out_handler.info("done");
}
