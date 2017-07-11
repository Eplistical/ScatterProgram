#ifndef _SCATTER_MPI_HELPER_HPP
#define _SCATTER_MPI_HELPER_HPP

#include "mpier.hpp"
#include <string>
#include "vars.hpp"
#include "io.hpp"
#include "grid.hpp"
#include "surfaces.hpp"
#include "rem.hpp"

namespace scatter 
{
	void inline bcast_vars(void) 
	{
		// broadcast rem para
		mpier::bcast(0, rem::jobname, rem::jobtype, rem::loaddat, rem::dim, rem::dim2, rem::hbar, rem::kT, rem::Gamma0);
		// broadcast grid para
		mpier::bcast(0, grid::rmin, grid::rmax, grid::Nr, grid::bandwidth, grid::derange);
		// broadcast surfaces para
		mpier::bcast(0, surfaces::surfnum, surfaces::cutoff_gamma);

		std::string tmp;
		surfaces::gammamode.resize(rem::dim);
		for (size_t i = 0; i < rem::dim; ++i) {
			if (mpier::master)
				tmp = enumspace::surfmode_dict.right.at(surfaces::gammamode[i]);
			mpier::bcast(0, tmp);
			surfaces::gammamode[i] = enumspace::surfmode_dict.left.at(tmp);
		}
		surfaces::gammapara.resize(rem::dim);
		for (size_t i = 0; i < rem::dim; ++i) {
			mpier::bcast(0, surfaces::gammapara[i]);
		}

		surfaces::surfmode.resize(rem::dim * surfaces::surfnum);
		for (size_t i = 0; i < rem::dim * surfaces::surfnum; ++i) {
			if (mpier::master)
				tmp = enumspace::surfmode_dict.right.at(surfaces::surfmode[i]);
			mpier::bcast(0, tmp);
			surfaces::surfmode[i] = enumspace::surfmode_dict.left.at(tmp);
		}

		surfaces::surfpara.resize(rem::dim * surfaces::surfnum);
		for (size_t i = 0; i < rem::dim * surfaces::surfnum; ++i) {
			mpier::bcast(0, surfaces::surfpara[i]);
		}
	}
};

#endif
