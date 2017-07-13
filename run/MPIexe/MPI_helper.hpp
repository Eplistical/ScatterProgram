#ifndef _SCATTER_MPI_HELPER_HPP
#define _SCATTER_MPI_HELPER_HPP
// helper functions in MPI
#include <iostream>

#include "types.hpp"
#include <string>
#include "MPIer.hpp"
#include "vars.hpp"
#include "io.hpp"
#include "grid.hpp"
#include "surfaces.hpp"
#include "rem.hpp"

namespace scatter 
{
	VOID_T inline bcast_vars(void) 
	{
		// broadcast rem para
		MPIer::bcast(0, rem::jobname, rem::jobtype, rem::loaddat, rem::dim, rem::dim2, rem::hbar, rem::kT, rem::Gamma0);
		// broadcast grid para
		MPIer::bcast(0, grid::rmin, grid::rmax, grid::Nr, grid::bandwidth, grid::derange);
		// broadcast surfaces para
		MPIer::bcast(0, surfaces::surfnum, surfaces::cutoff_gamma);

		STRING_T tmp;
		surfaces::gammamode.resize(rem::dim);
		for (UINT_T i = 0; i < rem::dim; ++i) {
			if (MPIer::master)
				tmp = enumspace::surfmode_dict.right.at(surfaces::gammamode[i]);
			MPIer::bcast(0, tmp);
			surfaces::gammamode[i] = enumspace::surfmode_dict.left.at(tmp);
		}
		surfaces::gammapara.resize(rem::dim);
		for (UINT_T i = 0; i < rem::dim; ++i) {
			MPIer::bcast(0, surfaces::gammapara[i]);
		}

		surfaces::surfmode.resize(rem::dim * surfaces::surfnum);
		for (UINT_T i = 0; i < rem::dim * surfaces::surfnum; ++i) {
			if (MPIer::master)
				tmp = enumspace::surfmode_dict.right.at(surfaces::surfmode[i]);
			MPIer::bcast(0, tmp);
			surfaces::surfmode[i] = enumspace::surfmode_dict.left.at(tmp);
		}

		surfaces::surfpara.resize(rem::dim * surfaces::surfnum);
		for (UINT_T i = 0; i < rem::dim * surfaces::surfnum; ++i) {
			MPIer::bcast(0, surfaces::surfpara[i]);
		}

		// broadcast simulation para
		MPIer::bcast(0, simulation::mass, simulation::omega, simulation::Ntraj, simulation::Nstep, simulation::EndT, 
						simulation::dt, simulation::Anastep, simulation::inittemp, simulation::vibstate, simulation::elestate);
	}
};

#endif
